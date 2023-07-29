from random import randint
import my_sm3
import binascii
from gmpy2 import invert
from my_sm3 import SM3
import math

#椭圆曲线加法
def addition(x1,y1,x2,y2,a,p):
    if x1==x2 and y1==p-y2:
        return False
    if x1!=x2:
        lamda = ((y2-y1)*invert(x2-x1,p))%p
    else :
        lamda=(((3*x1*x1+a)%p)*invert(2*y1, p))%p
    
    x3=(lamda*lamda-x1-x2)%p
    y3=(lamda*(x1-x3)-y1)%p
    return x3,y3

#定义点乘
def mul_point(x,y,k,a,p):
    k = bin(k)[2:]
    qx, qy = x,y
    for i in range(1,len(k)):
        qx,qy=addition(qx,qy,qx,qy,a,p)
        if k[i] == '1':
            qx,qy=addition(qx,qy,x,y,a,p)
    return qx,qy

#KDF
def kdf(z,klen):
    c_t= 1
    k=''
    for i in range(math.ceil(klen/256)):
        k = k+SM3(hex(int(z+'{:032b}'.format(c_t),2))[2:])
        c_t = c_t+1
    k='0'*((256-(len(bin(int(k,16))[2:])%256))%256)+bin(int(k,16))[2:]
    return k[:klen]

#192bit
p=0xBDB6F4FE3E8B1D9E0DA8C0D46F4C318CEFE4AFE3B6B8551F 
a=0xBB8E5E8FBC115E139FE6A814FE48AAA6F0ADA1AA5DF91985
b=0x1854BEBDC31B21B7AEFC80AB0ECD10D5B1B3308E6DBF11C1
n=0xBDB6F4FE3E8B1D9E0DA8C0D40FC962195DFAE76F56564677
gx=0x4AD5F7048DE709AD51236DE65E4D4B482C836DC6E4106640
gy=0x02BB3A02D4AAADACAE24817A4CA3A1B014B5270432DB27D2

db=randint(1,n-1)
xb,yb=mul_point(gx,gy,db,a,p)

#sm2加密算法，接受str类型参数
def sm2_encrypt(m:str):
    plen=len(hex(p)[2:])
    m='0'*((4-(len(bin(int(m.encode().hex(),16))[2:])%4))%4)+bin(int(m.encode().hex(),16))[2:]
    klen=len(m)
    while True:
        k=randint(1,n)
        print("密钥k = ",k)
        while k==db:
            k=randint(1, n)
        x2,y2=mul_point(xb, yb, k, a, p)
        x2,y2='{:0256b}'.format(x2),'{:0256b}'.format(y2)
        t=kdf(x2+y2, klen)
        if int(t,2)!=0:
            break
    x1,y1=mul_point(gx, gy, k, a, p)
    x1,y1=(plen-len(hex(x1)[2:]))*'0'+hex(x1)[2:],(plen-len(hex(y1)[2:]))*'0'+hex(y1)[2:]
    c1='04'+x1+y1
    c2=((klen//4)-len(hex(int(m,2)^int(t,2))[2:]))*'0'+hex(int(m,2)^int(t,2))[2:]
    c3=SM3(hex(int(x2+m+y2,2))[2:])
    return c1,c2,c3

def sm2_decrypt(c1,c2,c3):
    c1=c1[2:]
    x1,y1=int(c1[:len(c1)//2],16),int(c1[len(c1)//2:],16)
    if pow(y1,2,p)!=(pow(x1,3,p)+a*x1+b)%p:
        return False
    x2,y2=mul_point(x1, y1, db, a, p)
    x2,y2='{:0256b}'.format(x2),'{:0256b}'.format(y2)
    klen=len(c2)*4
    t=kdf(x2+y2, klen)
    if int(t,2)==0:
        return False
    m='0'*(klen-len(bin(int(c2,16)^int(t,2))[2:]))+bin(int(c2,16)^int(t,2))[2:]
    u=SM3(hex(int(x2+m+y2,2))[2:])
    if u!=c3:
        return False
    return hex(int(m,2))[2:]

m="SDUA627"
print("加密后密文为:")
c1,c2,c3=sm2_encrypt(m)
print(c1+c2+c3)
m2=sm2_decrypt(c1,c2,c3)
m2=binascii.a2b_hex(m2)
print("解密后明文为：\n",m2)
