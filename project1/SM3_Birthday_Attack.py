import random
import time

# 偏移向量
IV = [0x7380166F, 0x4914B2B9, 0x172442D7, 0xDA8A0600, 0xA96F30BC, 0x163138AA, 0xE38DEE4D, 0xB0FB0E4E]
T = [0x79cc4519, 0x7a879d8a]

def Shift(X, i):
    i = i % 32
    return ((X << i) & 0xFFFFFFFF) | ((X & 0xFFFFFFFF) >> (32 - i))

def FF(X, Y, Z, j):
    if j >= 0 and j <= 15:
        return X ^ Y ^ Z
    else:
        return ((X & Y) | (X & Z) | (Y & Z))

def GG(X, Y, Z, j):
    if j >= 0 and j <= 15:
        return X ^ Y ^ Z
    else:
        return ((X & Y) | (~X & Z))

def P0(X):
    return X ^ Shift(X, 9) ^ Shift(X, 17)

def P1(X):
    return X ^ Shift(X, 15) ^ Shift(X, 23)

def SM3_T1(j):
    if j >= 0 and j <= 15:
        return T[0]
    else:
        return T[1]

# 填充
def padding(message):
    m = bin(int(message, 16))[2:]
    if len(m) != len(message) * 4:
        m = '0' * (len(message) * 4 - len(m)) + m
    l = len(m)
    l_bin = '0' * (64 - len(bin(l)[2:])) + bin(l)[2:]
    m = m + '1'
    m = m + '0' * (448 - len(m) % 512) + l_bin
    m = hex(int(m, 2))[2:]
    return m

# 输入字符串
def Input(m):
    n = len(m) // 128
    M = []
    for i in range(int(n)):
        M.append(m[128 * i:128 * (i + 1)])
    return M

# 消息扩展
def Expand(M, n):
    W = []
    W1 = []
    for i in range(16):
        W.append(int(M[n][0 + 8 * i:8 + 8 * i], 16))
    for j in range(16, 68):
        t1 = Shift(W[j - 3], 15)
        t2 = Shift(W[j - 13], 7) ^ W[j - 6]
        t3 = W[j - 16] ^ W[j - 9]
        t4 = P1(t1 ^ t3)
        t5 = t4 ^ t2
        W.append(t5)
    for k in range(64):
        W1.append(W[k] ^ W[k + 4])
    Wstr = ''
    W_str = ''
    for x in W:
        Wstr += (hex(x)[2:] + ' ')
    for x in W1:
        W_str += (hex(x)[2:] + ' ')
    return W, W1

def CF(V, M, i):
    # reg init, set ABCDEFGH = V0
    A, B, C, D, E, F, G, H = V[i]
    W, W1 = Expand(M, i)
    for j in range(64):
        SS1 = Shift((Shift(A, 12) + E + Shift(SM3_T1(j), j % 32)) % (2 ** 32), 7)
        SS2 = SS1 ^ Shift(A, 12)
        TT1 = (FF(A, B, C, j) + D + SS2 + W1[j]) % (2 ** 32)
        TT2 = (GG(E, F, G, j) + H + SS1 + W[j]) % (2 ** 32)
        D = C
        C = Shift(B, 9)
        B = A
        A = TT1
        H = G
        G = Shift(F, 19)
        F = E
        E = P0(TT2)

    a, b, c, d, e, f, g, h = V[i]
    # update V
    V_ = [a ^ A, b ^ B, c ^ C, d ^ D, e ^ E, f ^ F, g ^ G, h ^ H]
    return V_

# SM3hash
def SM3(message):
    n = len(message)
    V = []
    V.append(IV)
    for i in range(n):
        V.append(CF(V, message, i))
    return V[n]

#SM3生日攻击
def SM3_birthday_attack(n):
    cipher = set()
    while True:
        list_r = random.randint(0, pow(2, 64))
        m = padding(str(list_r))
        V = SM3(Input(m))
        text = ""
        for x in V:
            text += hex(x)[2:]
        j = text[:n]
        if j in cipher:
            print("碰撞成功！碰撞的公共前缀为：", j)
            break
        else:
            cipher.add(j)
    #print(text)
    print("SM3生日攻击过程中，有以下杂凑值")
    for i in V:
        print(hex(i))
    print("V:",V)
    #print(j)
    #print("text:",text)
    #print("cipher:",cipher)

print("SM3生日攻击实现:")
start = time.time()
SM3_birthday_attack(8)
print("碰撞的长度为:",32)
end = time.time()
print("Time:%.3fs" % (end - start))
