![](Shanda.webp)

<!-- ********************* Chapter1 ********************* -->

## 1 分工表格

---

<center>

| 姓名  |      学号      | 分工        |
|:---:|:------------:|-----------|
| 刘晨曦 | 202100460042 | SHA256实现  |
| 卢梓宁 | 202100460043 | SHA256优化  |
| 陈辉华 | 202100460044 | SHA256长度攻击 |

</center>

<!-- ********************* Chapter2 ********************* -->

## 2 SHA256算法

---

SHA-256是一种密码学哈希函数，它属于SHA-2（安全哈希算法2系列）家族的一员。SHA-256可以将任意长度的消息转换为一个256位的固定长度哈希值。

SHA-256的应用非常广泛，特别是在密码学中。它被用于生成消息摘要，用于验证数据的完整性和真实性。哈希算法的一个重要性质是它是单向的，即无法从哈希值反推出原始消息。因此，SHA-256可以用于存储敏感数据的哈希值，而不用担心泄露原始数据。

SHA-256的输出是一个256位的二进制序列，通常以十六进制表示。由于其固定长度和低碰撞风险，SHA-256被广泛用于数字签名、密码校验和认证、密码存储等各种密码学应用。它在比特币和其他加密货币的区块链技术中也有重要的应用。

<!-- ********************* Chapter3 ********************* -->

## 3 SHA256长度攻击

---
### 3.1 攻击原理

SHA-256是一种哈希函数，它被广泛用于各种安全应用中。然而，尽管SHA-256在实际中被认为是一个相当安全的哈希函数，但它并不是绝对安全的，可能存在长度攻击。

长度攻击是一种针对哈希函数的攻击方式，其中攻击者试图找到两个不同的输入，它们产生相同的哈希值。在哈希函数中，输入的长度是一个重要的因素，因为改变输入的长度可能会导致哈希结果的改变。长度攻击利用了这个性质。

SHA-256使用了Merkle-Damgård结构，它将消息分成固定大小的块，并对每个块进行处理。在进行最终哈希计算之前，SHA-256会对消息进行填充以满足块的大小要求。填充的方式是在消息末尾添加比特，使得填充后的消息长度达到一个特定的值。

在长度攻击中，攻击者试图找到两个具有相同哈希值的消息，其中一个消息是正常长度的，而另一个消息是经过特定修改的，其长度稍微超过了块大小，从而导致填充方式的变化。

攻击者通过针对不同长度的消息进行哈希计算，并观察哈希值的变化，试图找到任意两个哈希值相同的消息。通过选择适当的修改，攻击者可以通过产生的填充差异来创建哈希冲突，从而达到绕过哈希函数的目的。

为了防御长度攻击，一种常见的做法是使用消息认证码(MAC)或者使用与消息长度无关的哈希函数，如SHA-3。此外，使用盐值(salt)或密钥增加了安全性，因为攻击者很难预测盐值或密钥对填充方式的影响。

### 3.2 攻击实现

因为篇幅原因，核心代码如下
```
def SHA256_Hash_Attack(h, s, mlen):
    H = [int(h[8 * i:8 * i + 8], base=16) for i in range(len(h) // 8)]
    # print(H)
    M = Padding_Attack(s, mlen)
    W = Initialize(M)
    H = Iteration(W, H)
    SHA256 = ''
    for i in H:
        SHA256 = SHA256 + i.to_bytes(4, byteorder='big').hex()
    return SHA256
```

<!-- ********************* Chapter4 ********************* -->

## 4 实验结果

---
实验前置条件：调用python中的hashlib库

实验结果附图如下：  
**SHA256 Length Attack** 
> ![](SHA256_Length_Attack.png)