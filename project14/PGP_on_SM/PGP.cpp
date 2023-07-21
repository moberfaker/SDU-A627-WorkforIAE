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
	PGPData���ݱ���ʽ��
		  8    bytes: uint8_t[]  UserID
		  8    bytes: size_t	 EncMessage_Len		���ܺ���Ϣ����
		  ?    bytes: uint8_t[]	 Message
		16+96  bytes: uint8_t[]  EncSessionKey
*/
void PGP::SendData() {
	if (this->RecverPubKey.pubX == NULL) {
		cout << "δ���ý��շ���Կ!" << endl;
		return;
	}
	else if (this->messageLen == 0) {
		cout << "û����Ϣ��Ҫ����!" << endl;
		return;
	}
	
	this->PGPmessage = (uint8_t*)malloc(16 + 2048);
	uint8_t* Point = this->PGPmessage;

	//��������ϢUserID
	memcpy(Point, this->UserID, USERID_BYTE);
	Point += USERID_BYTE;

	//�������SM4�Ự��Կ
	random_bytes_default(this->SessionKey, SM4_blockS);

	//ʹ�ûỰ��Կ��message����
	Paddata(&this->message, &this->messageLen, SM4_blockS);
	memcpy(Point, &this->messageLen, sizeof(size_t));
	Point += sizeof(size_t);
	this->PGPmessageLen += sizeof(size_t);

	sm4Enc(this->message, this->messageLen, Point, this->SessionKey);
	Point += this->messageLen;
	this->PGPmessageLen += this->messageLen;

	//ʹ��Reciver�Ĺ�Կ���ܻỰ��Կ
	sm2_encrypt(this->SessionKey, SM4_blockS, &this->RecverPubKey, Point);
	this->PGPmessageLen += 96 + 16;

}

/*
	PGPData���ݱ���ʽ��
		  8    bytes: uint8_t[]  UserID		
		  8    bytes: size_t	 EncMessage_Len		���ܺ���Ϣ����
		  ?    bytes: uint8_t[]	 Message
		16+96  bytes: uint8_t[]  EncSessionKey
*/
void PGP::ReadData(uint8_t* PGPmessage,size_t PGPmessageLen) {
	if (this->sm2Key.priKey == NULL) {
		cout << "��δ����sm2˽Կ��" << endl;
		return;
	}
	
	uint8_t* Point = PGPmessage;
	
	//��ȡ��������Ϣ
	memcpy(this->RecvUserID, Point, USERID_BYTE);
	Point += USERID_BYTE;

	//��ȡEncMessage_Len
	memcpy(&this->messageLen, Point, sizeof(size_t));
	Point += sizeof(size_t);

	//��ȡEncMessage
	uint8_t* cipherText = (uint8_t*)malloc(this->messageLen);
	memcpy(cipherText, Point, this->messageLen);
	Point += this->messageLen;

	//��ȡEncSessionKey
	uint8_t EncSessionKey[16 + 96];
	memcpy(EncSessionKey, Point, 16 + 96);
	sm2_decrypt(EncSessionKey, 16 + 96, this->sm2Key.priKey, this->sm2Key.priKeyLen, this->SessionKey);

	//����SessionKey����EncMessage��ȡMessage
	this->message = (uint8_t*)malloc(this->messageLen);
	sm4Dec(cipherText, this->messageLen, this->message, this->SessionKey);
}

void PGP::ShowData(){
	cout << "\n+++++++++++++++++++++++ " << this->UserID << " +++++++++++++++++++++++\n";
	cout << "From " << this->RecvUserID << ":\n\t" << this->message << endl;
	cout << "++++++++++++++++++++++++" << string(strlen((const char*)this->UserID), '+') << "++++++++++++++++++++++++\n";
}