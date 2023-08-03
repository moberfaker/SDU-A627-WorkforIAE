<div align="center">
<img src=https://img1.baidu.com/it/u=3783559290,1144114418&fm=253&fmt=auto&app=138&f=JPEG width=60% height="60%"/>
</div>

<!-- ********************* Chapter1 ********************* -->

## 1 分工表格
<center>

| 姓名  |      学号      | 分工 |
|:---:|:------------:|----|
| 刘晨曦 | 202100460042 | SM3代码实现 |
| 卢梓宁 | 202100460043 | 生日攻击实现 |
| 陈辉华 | 202100460044 | 生日攻击实现|

</center>

<!-- ********************* Chapter2 ********************* -->

## 2 生日攻击

#### 2.1 攻击原理及分析 
 利用生日攻击找前n比特碰撞​ 随机生成字符串消息M，我们计算H=SM3(M)，如果我们只考虑前n比特，那么当我们遍历2^(n/2)中可能时，找到至少一对匹配的概率大于0.5. 根据这个原理我们可以实现 sm3 前nbit的碰撞

假设存在M‘，有SM3(M)=SM3(M')，那么我们在前2^n比特范围内找到M'的概率是1/2^n,但是我们找两个数m1和m2，满足m1-m2 = M’的概率则是（C2n，2）/2^n。
<!--**【代码实现】** -->


#### 2.2 代码实现
**【SM3实现】**
```
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
```
**【SM3生日攻击】**
```
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

print("SM3_birthday_attack:")
start = time.time()
SM3_birthday_attack(4)
end = time.time()
print("Time:%.3fs" % (end - start))
```

<!-- ********************* Chapter3 ********************* -->

## 3 运行结果
对于我们给出的SM3 24bit的碰撞，时间开销大概为15s左右，
而16bit的碰撞只需要0.0542s。
> ![](res.png)  
> <!-- 与md文件同一目录下的foldername文件夹，里的1.png图片 -->