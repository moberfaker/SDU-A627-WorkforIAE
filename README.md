# 2023 SDU-A627-WorkforIAE 创新创业实践项目

队伍成员:

     姓名   学号
    刘晨曦 202100460042
    卢梓宁 202100460043
    陈辉华 202100460044


## 项目总结
本小组为温隆老师同意用信安作品赛项目作为结课作业的小组，除此之外，我们额外做了9个我们觉得有意思的项目。以下是我们的项目汇总

## PROJECT 1：
### PROJECT NAME：implement the naive birthday attack of reduced SM3
### PROJECT DIVISION：
<center>

| 姓名  |      学号      | 分工 |
|:---:|:------------:|----|
| 刘晨曦 | 202100460042 | SM3代码实现 |
| 卢梓宁 | 202100460043 | 生日攻击实现 |
| 陈辉华 | 202100460044 | 生日攻击实现|

</center>

### PROJECT SUMMARY：
实验测试得到sm3的不同长度的生日碰撞所需要的时间开销，具体为
16bit的碰撞需要0.130s,24bit需要4,476s而32bit就需要66.138s。
<center>

| 碰撞长度  |      碰撞结果      | 时间开销 |
|:---:|:------------:|----|
| 16bit |  bb41 | 0.130s |
| 24bit | 9a88e8 |  4.476s |
| 32bit | 6b571c4f | 66.138s|

</center>

> ![](project1/16bit.png) 
> ![](project1/24bit.png) 
> ![](project1/32bit.png) 



## PROJECT 2：
### PROJECT NAME：implement the Rho method of reduced SM3
### PROJECT DIVISION：
<center>

| 姓名  |      学号      | 分工 |
|:---:|:------------:|----|
| 刘晨曦 | 202100460042 | Rho碰撞实现 |
| 卢梓宁 | 202100460043 | Rho碰撞实现 |
| 陈辉华 | 202100460044 | Rho碰撞实现 |

</center>

### PROJECT SUMMARY：
实验通过不断计算一个随机数的SM3杂凑值，最终形成一个闭环，最终找到符合条件的碰撞。
具体的实验结果如下，打印出找到碰撞的时候环中的元素。
<center>

| 碰撞长度  |      碰撞结果      | 时间开销 |
|:---:|:------------:|----|
| 16bit |  6ef9... | 0.006s |
| 24bit | e7511a... |  5.114s |
</center>

> ![](project2/16bit.png) 
> ![](project2/24bit.png) 


## PROJECT 3：
### PROJECT NAME：implement length extension attack for SM3, SHA256, etc.
### PROJECT DIVISION：
<center>

| 姓名  |      学号      | 分工        |
|:---:|:------------:|-----------|
| 刘晨曦 | 202100460042 | SHA256实现  |
| 卢梓宁 | 202100460043 | SHA256优化  |
| 陈辉华 | 202100460044 | SHA256长度攻击 |

</center>

### PROJECT SUMMARY：
<center>
根据SHA-256的Merkle-Damgard结构，在original message的填充信息之后添加new message，通过适当的bit修改进行生日碰撞，构造相同的哈希值进行长度扩展攻击。
</center>

> ![](project3/SHA256_Length_Attack.png)  


## PROJECT 4：
### PROJECT NAME：do your best to optimize SM3 implementation (software)
### PROJECT DIVISION：
<center>

| 姓名  |      学号      | 分工    |
|:---:|:------------:|-------|
| 刘晨曦 | 202100460042 | SM3实现 |
| 卢梓宁 | 202100460043 | SM3优化 |
| 陈辉华 | 202100460044 | SM3优化 |

</center>

### PROJECT SUMMARY：
<center>
主要通过4种优化方法，即消息扩展优化、加密结构优化、加密步骤优化、预处理常数优化。

将SM3优化结果如下，且对算法进行分析，在消息越长的时候加速比越大。
</center>

> ![](project4/SM3_init_out.png) 
> ![](project4/SM3_quick_out.png)  

## PROJECT 9：
### PROJECT NAME：AES / SM4 software implementation
### PROJECT DIVISION：
<center>

| 姓名  |      学号      | 分工 |
|:---:|:------------:|----|
| 刘晨曦 | 202100460042 | 吞吐优化 |
| 卢梓宁 | 202100460043 | 代码编写 |
| 陈辉华 | 202100460044 | 代码优化 |

</center>

### PROJECT SUMMARY：
<center>
通过查表优化、OMP并行处理、循环展开、模运算优化和指令优化加速SM4加解密。

对各个阶段进行运行时间(ns)和吞吐量测试(Gbps),单次SM4运行时间达到14ns，吞吐量约8Gbps。
</center>

> ![](project9/test.jpg) 


## PROJECT 11：
### PROJECT NAME：AES / SM4 software implementation
### PROJECT DIVISION：
<center>

| 姓名  |      学号      | 分工 |
|:---:|:------------:|----|
| 刘晨曦 | 202100460042 | SM2加密 |
| 卢梓宁 | 202100460043 | SM2密钥分发 |
| 陈辉华 | 202100460044 | SM2签名 |

</center>

### PROJECT SUMMARY：
本实验用sm3作为杂凑函数，实现了sm2的加密解密，签名验签以及密钥交换。具体的结果如以下截图。

加密消息"SDUA627"得到结果如下
> ![](project11/enres.png) 

对消息"SDUA627"进行签名和验签
> ![](project11/signres.png) 

密钥交换得到结果如下，可以看到A和B得到了一个一样的密钥
> ![](project11/keyres.png)
  
## PROJECT 14：
### PROJECT NAME：
### Implement a PGP scheme with SM2 
### PROJECT DIVISION：
<center>

| 姓名  |      学号      | 分工 |
|:---:|:------------:|----|
| 刘晨曦 | 202100460042 | PGP代码编写与测试 |
| 卢梓宁 | 202100460043 | SM系列代码编写 |
| 陈辉华 | 202100460044 | PRG算法编写与代码优化 |

</center>

### PROJECT SUMMARY：

本项目的PGP协议基于国密算法原语SM2/SM3/SM4，皆通过c语言自主实现与优化，随机数生成器PRG基于c++密码学库bcrypt.lib。具体框架如下：
> ![PGP框架](project14/PGP框架.jpg "PGP框架")

具体算法应用涉及UserID、时间戳等信息，需要对PGP报文进行设计:
```
	PGPData数据报格式：
		  8    bytes: uint8_t[]  UserID		
		  8    bytes: size_t	 EncMessage_Len		加密后消息长度
		  ?    bytes: uint8_t[]  Message
		16+96  bytes: uint8_t[]  EncSessionKey
```

模拟测试Alice与Bob之间的PGP通信：

> ![PGP模拟测试](project14/PGP模拟测试.jpg "PGP模拟测试")

测试在1024Bytes数据量下PGP协议速度与吞吐量：

> ![PGP速度_吞吐测试](project14/PGP速度_吞吐测试.jpg "PGP速度_吞吐测试")

## PROJECT 15 && 16
### implement sm2 2P sign with real network communication
### implement sm2 2P decrypt with real network communication
<center>

| 姓名  |      学号      | 分工 |
|:---:|:------------:|----|
| 刘晨曦 | 202100460042 | TCP代码编写和测试 |
| 卢梓宁 | 202100460043 | SM系列代码编写 |
| 陈辉华 | 202100460044 | PRG算法编写与代码优化 |

</center>

### PROJECT SUMMARY：

本项目sm2算法基于c语言miracl大数算法，并对miracl的内部函数进行相关优化和修改。使秘钥生成更加随机的同时对sm2加解密速度有所提升

网络传输实现基于c语言库WinSocket2.h，实现了能够处理多个服务请求的多线程服务端。服务端分别为：

-Project 15: 向链接的客户端发送本地sm2公钥，并接收客户端发送的加密信息，解密后输出。
-Project 16: 接收客户端的公钥和带签名的消息，并通过公钥对消息进行验证，验证后输出验证结果。

相关可执行文件分别在project文件夹中，以下为测试结果：



-project 15: 签名0.7ms，验证0.9ms。

> ![SM2签名测试](project15/SM2签名测试.jpg "SM2签名测试")

-Project 16: 加密1.5ms，解密0.9ms。

> ![SM2加密测试](project16/SM2加密测试.jpg "SM2加密测试")

## 注
在每个项目中如果没有特殊说明，即可以直接运行，具体代码见具体的project文件夹。这里只展示实验结果和数据！
