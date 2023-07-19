import time

WordSizeBits=32     # 运算以32bit为单位

IV = [0x7380166F, 0x4914B2B9, 0x172442D7, 0xDA8A0600, 0xA96F30BC, 0x163138AA, 0xE38DEE4D, 0xB0FB0E4E]   # 初始256bitV0
T  = [0x79cc4519, 0x7a879d8a]

def FF16(X,Y,Z):
    return X^Y^Z
def FF64(X,Y,Z):
    return ((X & Y) | (X & Z) | (Y & Z))
def GG16(X,Y,Z):
    return X^Y^Z
def GG64(X,Y,Z):
    return ((X & Y) | (~X & Z))

def Extend(m):
    """
    :param m: 原始消息m
    :return: 消息扩展后的消息M
    """
    M = bin(int(m,16))[2:]
    if len(M) != len(m)*4:
        M = '0'*(len(m)*4-len(M)) + M
    l = len(M)
    l_bin = '0'*(64-len(bin(l)[2:])) + bin(l)[2:]
    M = M + '1'
    M = M + '0'*(448-len(M)%512) + l_bin
    M = hex(int(M,2))[2:]
    return M

def Divides(m):
    """
    :param m: 消息扩展后的消息m
    :return:  将消息m按512bit分组
    """
    n = len(m)/128
    M = []
    for i in range(int(n)):
        M.append(m[0+128*i:128+128*i])
    return M

def LEFT(X,i):
    i = i % 32
    return (( X << i ) & 0xFFFFFFFF) | (( X & 0xFFFFFFFF ) >> ( 32 - i ))
def P0(X):
    return X^LEFT(X,9)^LEFT(X,17)
def OneRound12(i,j,A,B,C,D,E,F,G,H,W,M):
    N = 2**WordSizeBits
    W.append(int(M[i][32 + 8 * j:40 + 8 * j], 16))
    SS2 = LEFT(A, 12)
    SS1 = LEFT((SS2 + E + LEFT(T[0], j % 32)) % (N), 7)
    SS2 = SS1 ^ SS2
    D = (FF16(A,B,C)+D+SS2+(W[j]^W[j+4]))%(N)
    H = (GG16(E,F,G)+H+SS1+W[j])%(N)
    B = LEFT(B,9)
    F = LEFT(F,19)
    H = P0(H)
    return A,B,C,D,E,F,G,H

def P1(X):
    return X^LEFT(X,15)^LEFT(X,23)
def OneRound16(i,j,A,B,C,D,E,F,G,H,W,M):
    N = 2**WordSizeBits
    W.append(P1(W[j - 12] ^ W[j - 5] ^ LEFT(W[j + 1], 15)) ^ LEFT(W[j - 9], 7) ^ W[j - 2])
    SS2 = LEFT(A, 12)
    SS1 = LEFT((SS2 + E + LEFT(T[0], j % 32)) % (N), 7)
    SS2 = SS1 ^ SS2
    D = (FF16(A,B,C)+D+SS2+(W[j]^W[j+4]))%(N)
    H = (GG16(E,F,G)+H+SS1+W[j])%(N)
    B = LEFT(B,9)
    F = LEFT(F,19)
    H = P0(H)
    return A,B,C,D,E,F,G,H

def OneRound64(i,j,A,B,C,D,E,F,G,H,W,M):
    N = 2**WordSizeBits
    W.append(P1(W[j - 12] ^ W[j - 5] ^ LEFT(W[j + 1], 15)) ^ LEFT(W[j - 9], 7) ^ W[j - 2])
    SS2 = LEFT(A, 12)
    SS1 = LEFT((SS2 + E + LEFT(T[1], j % 32)) % (N), 7)
    SS2 = SS1 ^ SS2
    D = (FF64(A,B,C)+D+SS2+(W[j]^W[j+4]))%(N)
    H = (GG64(E,F,G)+H+SS1+W[j])%(N)
    B = LEFT(B,9)
    F = LEFT(F,19)
    H = P0(H)
    return A,B,C,D,E,F,G,H

def Hash(V,M,i):
    A,B,C,D,E,F,G,H = V[i]
    W=[]
    W.append(int(M[i][0:8], 16))
    W.append(int(M[i][8:16], 16))
    W.append(int(M[i][16:32], 16))
    W.append(int(M[i][32:40], 16))
    for j in range(0, 12, 4):
        A,B,C,D,E,F,G,H = OneRound12(i,j+0,A,B,C,D,E,F,G,H,W,M)
        A,B,C,D,E,F,G,H = OneRound12(i,j+1,D,A,B,C,H,E,F,G,W,M)
        A,B,C,D,E,F,G,H = OneRound12(i,j+2,C,D,A,B,G,H,E,F,W,M)
        A,B,C,D,E,F,G,H = OneRound12(i,j+3,B,C,D,A,F,G,H,E,W,M)
    A, B, C, D, E, F, G, H = OneRound16(i, j + 3, B, C, D, A, F, G, H, E, W, M)
    A, B, C, D, E, F, G, H = OneRound16(i, j + 3, B, C, D, A, F, G, H, E, W, M)
    A, B, C, D, E, F, G, H = OneRound16(i, j + 3, B, C, D, A, F, G, H, E, W, M)
    A, B, C, D, E, F, G, H = OneRound16(i, j + 3, B, C, D, A, F, G, H, E, W, M)
    for j in range(16, 64, 4):
        A,B,C,D,E,F,G,H = OneRound64(i,j+0,A,B,C,D,E,F,G,H,W,M)
        A,B,C,D,E,F,G,H = OneRound64(i,j+1,D,A,B,C,H,E,F,G,W,M)
        A,B,C,D,E,F,G,H = OneRound64(i,j+2,C,D,A,B,G,H,E,F,W,M)
        A,B,C,D,E,F,G,H = OneRound64(i,j+3,B,C,D,A,F,G,H,E,W,M)
    a,b,c,d,e,f,g,h = V[i]
    V_new = [a^A,b^B,c^C,d^D,e^E,f^F,g^G,h^H]
    return V_new

def SM3(M):
    n = len(M)
    V = []
    V.append(IV)
    for i in range(0,n):
        V.append(Hash(V,M,i))
    return V[n]

def SM3_test(num):
    m = 2**num-1
    M = Extend(str(m))      # 消息扩展
    M_list = Divides(M)     # 分组
    Vn = SM3(M_list)        # 加密
    global Hash_value
    Hash_value = ''
    for x in Vn:
        Hash_value += "%08x" % x
    # print(Hash_value)

def main(num):
    N = 1000
    start = time.time()
    for i in range(N):
        SM3_test(num)
    end = time.time()
    runtime = (end - start)/1400
    print(f' m 为 {num}bit, 运行时间:', runtime * 1000, "ms")

if __name__ == '__main__':
    main(100)
    main(1000)
    main(10000)
