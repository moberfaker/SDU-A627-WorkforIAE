import time

WordSizeBits=32     # 运算以32bit为单位

IV = [0x7380166F, 0x4914B2B9, 0x172442D7, 0xDA8A0600, 0xA96F30BC, 0x163138AA, 0xE38DEE4D, 0xB0FB0E4E]
T  = [0x79cc4519, 0x7a879d8a]

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

def LEFT(X,i):                          # 循环左移
    i = i % 32
    return ((X<<i)&0xFFFFFFFF) | ((X&0xFFFFFFFF)>>(32-i))
def P1(X):
    return X^LEFT(X,15)^LEFT(X,23)
def Expand(M,n):                        # 获取W和W_
    W = []
    W_ = []
    for j in range(16):
        W.append(int(M[n][0+8*j:8+8*j],16))
    for j in range(16,68):
        W.append(P1(W[j-16]^W[j-9]^LEFT(W[j-3],15))^LEFT(W[j-13],7)^W[j-6])
    for j in range(64):
        W_.append(W[j]^W[j+4])
    return W,W_
def FF(X,Y,Z,j):
    if j>=0 and j<=15: return X ^ Y ^ Z
    else: return ((X & Y) | (X & Z) | (Y & Z))
def GG(X,Y,Z,j):
    if j>=0 and j<=15: return X ^ Y ^ Z
    else: return ((X & Y) | (~X & Z))
def TT(j):
    if j>=0 and j<=15: return T[0]
    else: return T[1]
def P0(X):
    return X^LEFT(X,9)^LEFT(X,17)
def Hash(V,M,i):
    A,B,C,D,E,F,G,H = V[i]
    W,W_ = Expand(M,i)
    for j in range(64):
        SS1 = LEFT((LEFT(A,12)+E+LEFT(TT(j),j%32))%(2**32),7)
        SS2 = SS1 ^ LEFT(A,12)
        TT1 = (FF(A,B,C,j)+D+SS2+W_[j])%(2**32)
        TT2 = (GG(E,F,G,j)+H+SS1+W[j])%(2**32)
        D = C
        C = LEFT(B,9)
        B = A
        A = TT1
        H = G
        G = LEFT(F,19)
        F = E
        E = P0(TT2)

    a,b,c,d,e,f,g,h = V[i]
    V_new = [a^A,b^B,c^C,d^D,e^E,f^F,g^G,h^H]
    return V_new

def SM3(M):
    n = len(M)
    V = []
    V.append(IV)
    for i in range(n):
        V.append(Hash(V,M,i))
    return V[n]


def SM3_test(num):
    m = 2**num
    M = Extend(str(m))      # 消息扩展
    M_list = Divides(M)     # 分组
    Vn = SM3(M_list)        # 加密
    global Hash_value
    Hash_value = ''
    for x in Vn:
        Hash_value += "%08x" % x
    print(Hash_value)

def main(num):
    N = 1
    start = time.time()
    for i in range(N):
        SM3_test(num)
    end = time.time()
    runtime = (end - start)/N
    print(f' m 为 {num}bit, 运行时间:', runtime * 1000, "ms")

if __name__ == '__main__':
    main(100)
    main(1000)
    main(10000)
