#include"PGP.h"
#include"tools.h"
#include<string.h>
#include"cryptoRandom.h"

PGP::PGP(const char userID[USERID_BYTE])
{
	if (strlen(userID) <= USERID_BYTE) {
		memcpy(this->UserID, (const uint8_t*)userID, strlen(userID));
		memset(this->UserID + strlen(userID), 0x00, 1);
	}	
	else {
		memcpy(this->UserID, (const uint8_t*)userID, USERID_BYTE);
		memset(this->UserID + USERID_BYTE - 1, 0x00, 1);
	}
		

	this->message = NULL;
	this->messageLen = 0;

	this->PGPmessage = NULL;
	this->PGPmessageLen = 0;


	this->RecverPubKey.pubXLen = 0;
	this->RecverPubKey.pubYLen = 0;
}


PGP::~PGP()
{
	//if (this->message)
	//	free(this->message);
	//if (this->PGPmessage)
	//	free(this->PGPmessage);
	//if (RecverPubKey.pubX)
	//	free(RecverPubKey.pubX);
	//if (RecverPubKey.pubY)
	//	free(RecverPubKey.pubY);
}


void PGP::Setmessage(uint8_t* data, size_t dataLen) {
	this->message = (uint8_t*)malloc(dataLen);
	memcpy(this->message, data, dataLen);
	this->messageLen = dataLen;
}


void PGP::SetRecverPubKey(SM2_PUBKEY* RecverPubKey) {
	memcpy(&this->RecverPubKey.pubX, RecverPubKey->pubX, RecverPubKey->pubXLen);
	memcpy(&this->RecverPubKey.pubY, RecverPubKey->pubY, RecverPubKey->pubYLen);
	this->RecverPubKey.pubXLen = RecverPubKey->pubXLen;
	this->RecverPubKey.pubYLen = RecverPubKey->pubYLen;
}


void PGP::BuildKey() {
	//Sleep(1000);
	sm2_keygen(&this->sm2Key);
}


SM2_PUBKEY* PGP::GetPubKey() {
	return &this->sm2Key.pubkey;
}


/*
	PGPData数据报格式：
		  8    bytes: uint8_t[]  UserID
		  8    bytes: size_t	 EncMessage_Len		加密后消息长度
		  ?    bytes: uint8_t[]	 Message
		16+96  bytes: uint8_t[]  EncSessionKey
*/
void PGP::SendData() {
	if (this->RecverPubKey.pubX == NULL) {
		cout << "未设置接收方公钥!" << endl;
		return;
	}
	else if (this->messageLen == 0) {
		cout << "没有信息需要发送!" << endl;
		return;
	}
	
	this->PGPmessage = (uint8_t*)malloc(16 + 2048);
	uint8_t* Point = this->PGPmessage;

	//添加身份信息UserID
	memcpy(Point, this->UserID, USERID_BYTE);
	Point += USERID_BYTE;

	//随机生成SM4会话秘钥
	random_bytes_default(this->SessionKey, SM4_blockS);

	//使用会话秘钥对message加密
	Paddata(&this->message, &this->messageLen, SM4_blockS);
	memcpy(Point, &this->messageLen, sizeof(size_t));
	Point += sizeof(size_t);
	this->PGPmessageLen += sizeof(size_t);

	sm4Enc(this->message, this->messageLen, Point, this->SessionKey);
	Point += this->messageLen;
	this->PGPmessageLen += this->messageLen;

	//使用Reciver的公钥加密会话秘钥
	sm2_encrypt(this->SessionKey, SM4_blockS, &this->RecverPubKey, Point);
	this->PGPmessageLen += 96 + 16;

}

/*
	PGPData数据报格式：
		  8    bytes: uint8_t[]  UserID		
		  8    bytes: size_t	 EncMessage_Len		加密后消息长度
		  ?    bytes: uint8_t[]	 Message
		16+96  bytes: uint8_t[]  EncSessionKey
*/
void PGP::ReadData(uint8_t* PGPmessage,size_t PGPmessageLen) {
	if (this->sm2Key.priKey == NULL) {
		cout << "尚未生成sm2私钥！" << endl;
		return;
	}
	
	uint8_t* Point = PGPmessage;
	
	//提取发件人信息
	memcpy(this->RecvUserID, Point, USERID_BYTE);
	Point += USERID_BYTE;

	//提取EncMessage_Len
	memcpy(&this->messageLen, Point, sizeof(size_t));
	Point += sizeof(size_t);

	//提取EncMessage
	uint8_t* cipherText = (uint8_t*)malloc(this->messageLen);
	memcpy(cipherText, Point, this->messageLen);
	Point += this->messageLen;

	//提取EncSessionKey
	uint8_t EncSessionKey[16 + 96];
	memcpy(EncSessionKey, Point, 16 + 96);
	sm2_decrypt(EncSessionKey, 16 + 96, this->sm2Key.priKey, this->sm2Key.priKeyLen, this->SessionKey);

	//利用SessionKey解密EncMessage获取Message
	this->message = (uint8_t*)malloc(this->messageLen);
	sm4Dec(cipherText, this->messageLen, this->message, this->SessionKey);
}

void PGP::ShowData(){
	cout << "\n+++++++++++++++++++++++ " << this->UserID << " +++++++++++++++++++++++\n";
	cout << "From " << this->RecvUserID << ":\n\t" << this->message << endl;
	cout << "++++++++++++++++++++++++" << string(strlen((const char*)this->UserID), '+') << "++++++++++++++++++++++++\n";
}