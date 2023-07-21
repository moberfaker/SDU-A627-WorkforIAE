#ifndef PGP_H
#define PGP_H

#include<iostream>
#include<string>
#include "sm2.h"
#include "sm3.h"
#include "sm4.h"

using namespace std;

#define USERID_BYTE 16

class PGP
{
public:
	PGP(const char userID[USERID_BYTE]);
	~PGP();
	
	void Setmessage(uint8_t* data, size_t dataLen);	//设置邮件信息内容
	void SetRecverPubKey(SM2_PUBKEY* RecverPubKey);	//设置收件人公钥
	void BuildKey();				//生成自己的公私钥SM2
	SM2_PUBKEY* GetPubKey();		//查看自己的公钥

	void SendData();											//生成PGP处理后的邮件信息
	void ReadData(uint8_t* PGPmessage, size_t PGPmessageLen);	//翻译PGP处理后的邮件信息

	void ShowData();				//输出接收到的信息

	uint8_t UserID[USERID_BYTE];	//用户ID
	SM2_PUBKEY RecverPubKey;	//接收者SM2公钥
	uint8_t* message;			//普通邮件信息
	uint8_t* PGPmessage;		//PGP处理后的邮件信息
	size_t messageLen;
	size_t PGPmessageLen;
	uint8_t RecvUserID[USERID_BYTE];//来信用户ID
private:
	SM2_KEY sm2Key;			//user的SM2公私钥
	uint8_t SessionKey[16];	//SM4会话秘钥
};

#endif
