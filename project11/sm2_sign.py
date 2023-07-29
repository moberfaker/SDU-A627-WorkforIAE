import math
from gmpy2 import invert
import random
from my_sm3 import SM3
import binascii
#256bit参数
p=0x8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3
a=0x787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498
b=0x63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A
n=0x8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7
gx=0x421DEBD61B62EAB6746434EBC3CC315E32220B3Badd50BDC4C4E6C147FEDD43D
gy=0x0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2

da=0x128B2FA8BD433C6C068C8D803DFF79792A519A55171B1B650C23661D15897263

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

def mul_point(x,y,k):
    k=bin(k)[2:]
    qx,qy=x,y
    for i in range(1,len(k)):
        qx,qy=addition(qx, qy, qx, qy)
        if k[i]=='1':
            qx,qy=addition(qx, qy, x, y)
    return qx,qy

def Sign(m,Za):
    m1=Za+m
    e=SM3(m1)
    #k=randint(1,n)
    k=0x6CB28D99385C175C94F94E934817663FC176D925DD72B727260DBAAE1FB2F96F
    x1,y1=mul_point(gx,gy,k)
    r=(int(e,16)+x1)%n
    s=(invert(1+da,n)*(k-r*da))%n
    return (hex(r)[2:].upper(),hex(s)[2:].upper())

def Verify(r,s,Za,m,Pa):
    if int(r,16) not in range(1,n-1):
        return False
    if int(s,16) not in range(1,n-1):
        return False
    m1=Za+m
    e=SM3(m1)
    t=(int(r,16)+int(s,16))%n
    if t==0:
        return False
    x1,y1=mul_point(Pa[0],Pa[1],t)
    x2,y2=mul_point(gx,gy,int(s,16))
    x1,y1=addition(x2,y2,x1,y1)
    R=(int(e,16)+x1)%n
    if(hex(R)[2:].upper()==r):
        return True
    return False


Pax,Pay=mul_point(gx,gy,da)
Pa=(Pax,Pay)
m="SDUA627"
print("签名的消息为",m)
m=hex(int(binascii.b2a_hex(m.encode()).decode(),16)).upper()[2:]
IDa="A6271@sdu.edu.cn"
ida=hex(int(binascii.b2a_hex(IDa.encode()).decode(),16)).upper()[2:]
ENTLa='{:04X}'.format(len(ida)*4)
m1=ENTLa+ida+'{:064X}'.format(a)+'{:064X}'.format(b)+'{:064X}'.format(gx)+'{:064X}'.format(gy)+'{:064X}'.format(Pa[0])+'{:064X}'.format(Pa[1])
Za=hex(int(SM3(m1),16))[2:].upper()

Sign=Sign(m,Za)
res=["验证失败","验证成功"]
print(res[Verify(*Sign,Za,m,Pa)])