"""
    在签名的过程中签名需要消息m和Za
    验证签名需要签名、m、Za和公钥
"""
import math
from my_sm3 import SM3
from gmpy2 import invert
from random import randint
import binascii
import math

#前置参数 256bit




IDa="A6271@sdu.edu.cn"
IDb="A6272@sdu.edu.cn"

p=0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3
a=0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498
b=0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A
n=0x8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7
gx=0x421DEBD61B62EAB6746434EBC3CC315E32220B3Badd50BDC4C4E6C147FEDD43D
gy=0x0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2

da=0x6FCBA2EF9AE0AB902BC3BDE3FF915D44BA4CC78F88E2F8E7F8996D3B8CCEEDEE
db=0x5E35D7D3F3C54DBAC72E61819E730B019A84208CA3A35E4C2E353DFCCB2A3B53
w=math.ceil(math.ceil(math.log2(n))/2)-1
h=1
klen=128

#椭圆曲线上的加法
def addition(x1,y1,x2,y2):
    if x1==x2 and y1==p-y2:
        return False
    if x1!=x2:
        lamda=((y2-y1)*invert(x2-x1, p))%p
    else:
        lamda=(((3*x1*x1+a)%p)*invert(2*y1, p))%p
    x3=(lamda*lamda-x1-x2)%p
    y3=(lamda*(x1-x3)-y1)%p
    return x3,y3

#点乘
def mul_point(x,y,k):
    k=bin(k)[2:]
    qx,qy=x,y
    for i in range(1,len(k)):
        qx,qy=addition(qx, qy, qx, qy)
        if k[i]=='1':
            qx,qy=addition(qx, qy, x, y)
    return qx,qy

Pka=mul_point(gx,gy,da)
Pkb=mul_point(gx,gy,db)

#密钥分发
def KDF(z,klen):
    ct=1
    k=''
    for i in range(math.ceil(klen/256)):
        t=hex(int(z+'{:032b}'.format(ct),2))[2:]
        k=k+hex(int(SM3(t),16))[2:]
        ct=ct+1
    k='0'*((256-(len(bin(int(k,16))[2:])%256))%256)+bin(int(k,16))[2:]
    return k[:klen]

#B交换密钥
def Bkey_Exchange(*Ra):
    Rb = mul_point(gx, gy, rb)
    ida = hex(int(binascii.b2a_hex(IDa.encode()).decode(), 16)).upper()[2:]
    ENTLa = '{:04X}'.format(len(ida) * 4)
    ma = ENTLa + ida + '{:064X}'.format(a) + '{:064X}'.format(b) + '{:064X}'.format(gx) + '{:064X}'.format(
        gy) + '{:064X}'.format(Pka[0]) + '{:064X}'.format(Pka[1])
    Za = SM3(ma)
    idb = hex(int(binascii.b2a_hex(IDb.encode()).decode(), 16)).upper()[2:]
    ENTLb = '{:04X}'.format(len(idb) * 4)
    mb = ENTLb + idb + '{:064X}'.format(a) + '{:064X}'.format(b) + '{:064X}'.format(gx) + '{:064X}'.format(
        gy) + '{:064X}'.format(Pkb[0]) + '{:064X}'.format(Pkb[1])
    Zb = SM3(mb)
    x2=((1<<w)+(Rb[0]&((1<<w)-1)))%(1<<128)
    tb=(db+x2*rb)%n
    x1=((1<<w)+(Ra[0]&((1<<w)-1)))%(1<<128)
    x,y=mul_point(Ra[0],Ra[1],x1)
    x,y=addition(Pka[0],Pka[1],x,y)
    V=mul_point(x,y,h*tb)
    t1, t2 = '{:064X}'.format(V[0]), '{:064X}'.format(V[1])
    m=t1+t2+Za+Zb
    m='{:01024b}'.format(int(m,16))
    Kb=KDF(m,klen)
    return hex(int(Kb,2)).upper()[2:]
#A交换密钥
def Akey_Exchange(*Rb):
    Ra = mul_point(gx, gy, ra)
    ida = hex(int(binascii.b2a_hex(IDa.encode()).decode(), 16)).upper()[2:]
    ENTLa = '{:04X}'.format(len(ida) * 4)
    ma = ENTLa + ida + '{:064X}'.format(a) + '{:064X}'.format(b) + '{:064X}'.format(gx) + '{:064X}'.format(
        gy) + '{:064X}'.format(Pka[0]) + '{:064X}'.format(Pka[1])
    Za = SM3(ma)
    idb = hex(int(binascii.b2a_hex(IDb.encode()).decode(), 16)).upper()[2:]
    ENTLb = '{:04X}'.format(len(idb) * 4)
    mb = ENTLb + idb + '{:064X}'.format(a) + '{:064X}'.format(b) + '{:064X}'.format(gx) + '{:064X}'.format(
        gy) + '{:064X}'.format(Pkb[0]) + '{:064X}'.format(Pkb[1])
    Zb = SM3(mb)
    x1=((1<<w)+(Ra[0]&((1<<w)-1)))%(1<<128)
    ta=(da+x1*ra)%n
    x2=((1<<w)+(Rb[0]&((1<<w)-1)))%(1<<128)
    x,y=mul_point(Rb[0],Rb[1],x2)
    x,y=addition(Pkb[0],Pkb[1],x,y)
    U=mul_point(x,y,h*ta)
    t1, t2 = '{:064X}'.format(U[0]), '{:064X}'.format(U[1])
    m=t1+t2+Za+Zb
    m='{:01024b}'.format(int(m,16))
    Ka=KDF(m,klen)
    return hex(int(Ka, 2)).upper()[2:]

ra=randint(1,n)
print("ra = ",ra)
rb=randint(1,n)
print("rb = ",rb)
Ra=mul_point(gx,gy,ra)
Rb=mul_point(gx,gy,rb)
print("B交换得到的密钥为：",Bkey_Exchange(*Ra))
print("A交换得到的密钥为: ",Akey_Exchange(*Rb))