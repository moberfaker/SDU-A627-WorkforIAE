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
	
	void Setmessage(uint8_t* data, size_t dataLen);	//�����ʼ���Ϣ����
	void SetRecverPubKey(SM2_PUBKEY* RecverPubKey);	//�����ռ��˹�Կ
	void BuildKey();				//�����Լ��Ĺ�˽ԿSM2
	SM2_PUBKEY* GetPubKey();		//�鿴�Լ��Ĺ�Կ

	void SendData();											//����PGP�������ʼ���Ϣ
	void ReadData(uint8_t* PGPmessage, size_t PGPmessageLen);	//����PGP�������ʼ���Ϣ

	void ShowData();				//������յ�����Ϣ

	uint8_t UserID[USERID_BYTE];	//�û�ID
	SM2_PUBKEY RecverPubKey;	//������SM2��Կ
	uint8_t* message;			//��ͨ�ʼ���Ϣ
	uint8_t* PGPmessage;		//PGP�������ʼ���Ϣ
	size_t messageLen;
	size_t PGPmessageLen;
	uint8_t RecvUserID[USERID_BYTE];//�����û�ID
private:
	SM2_KEY sm2Key;			//user��SM2��˽Կ
	uint8_t SessionKey[16];	//SM4�Ự��Կ
};

#endif
