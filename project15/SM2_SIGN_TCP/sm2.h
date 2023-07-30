#ifndef SM2_H
#define SM2_H

#include "sm3.h"
#include "miracl/miracl.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		uint8_t pubX[32];
		uint8_t pubY[32];
		int pubXLen;
		int pubYLen;
	}SM2_PUBKEY;

	typedef struct {
		SM2_PUBKEY pubkey;
		uint8_t priKey[32];
		int priKeyLen;
	}SM2_KEY;

	int sm3_e(uint8_t* userid, int userid_len, SM2_PUBKEY pk, uint8_t* msg, int msg_len, uint8_t* e);
	/*
	���ܣ������û�ID����Կ��������ǩ������ǩ����ϢHASHֵ
	[����] userid�� �û�ID
	[����] userid_len�� userid���ֽ���
	[����] pubX�� ��Կ��X����
	[����] pubXLen: pubX���ֽ���
	[����] pubY�� ��Կ��Y����
	[����] pubYLen: pubY���ֽ���
	[����] msg��Ҫǩ������Ϣ
	[����] msg_len�� msg���ֽ���
	[���] e��32�ֽڣ�����ǩ������ǩ

	����ֵ��
			��1���ڴ治��
			  0���ɹ�
	*/

	void sm2_keygen(SM2_KEY* key);
	/*
	���ܣ�����SM2��˽Կ��
	[���] pubX��   ��Կ��X���꣬����32�ֽ���ǰ���0x00
	[���] pubXLen: pubX���ֽ�����32
	[���] pubY��   ��Կ��Y���꣬����32�ֽ���ǰ���0x00
	[���] pubYLen: pubY���ֽ�����32
	[���] priKey��˽Կ������32�ֽ���ǰ���0x00
	[���] priKeyLen�� priKey���ֽ�����32
	*/

	void sm2_sign(uint8_t* hash, int hashlen, uint8_t* priKey, int priKeyLen, uint8_t* cr, int* rlen, uint8_t* cs, int* slen);
	/*
	���ܣ�SM2ǩ��
	[����] hash��    sm3_e()�Ľ��
	[����] hashlen�� hash���ֽ�����ӦΪ32
	[����] priKey�� ˽Կ
	[����] priKeyLen�� priKeyLen���ֽ���

	[���] cr��  ǩ������ĵ�һ���֣�����32�ֽ���ǰ���0x00��
	[���] rlen��cr���ֽ�����32
	[���] cs��  ǩ������ĵڶ����֣�����32�ֽ���ǰ���0x00��
	[���] slen��cs���ֽ�����32
	*/

	int  sm2_verify(uint8_t* hash, int hashlen, uint8_t* cr, int rlen, uint8_t* cs, int slen, SM2_PUBKEY pk);
	/*
	���ܣ���֤SM2ǩ��
	[����] hash��    sm3_e()�Ľ��
	[����] hashlen�� hash���ֽ�����ӦΪ32
	[����] cr��  ǩ������ĵ�һ����
	[����] rlen��cr���ֽ���
	[����] cs��  ǩ������ĵڶ����֡�
	[����] slen��cs���ֽ���
	[����] pubX��   ��Կ��X����
	[����] pubXLen: pubX���ֽ�����������32�ֽ�
	[����] pubY��   ��Կ��Y����
	[����] pubYLen: pubY���ֽ�����������32�ֽ�

	����ֵ��
			0����֤ʧ��
			1����֤ͨ��
	*/

	int  sm2_encrypt(uint8_t* msg, int msglen, SM2_PUBKEY* pubKey, uint8_t* outmsg);
	/*
	���ܣ���SM2��Կ�������ݡ����ܽ�����������ݶ�96�ֽڣ�
	[����] msg     Ҫ���ܵ�����
	[����] msglen��msg���ֽ���
	[����] pubX��    ��Կ��X����
	[����] pubXLen:  pubX���ֽ�����������32�ֽ�
	[����] pubY��    ��Կ��Y����
	[����] pubYLen:  pubY���ֽ�����������32�ֽ�

	[���] outmsg: ���ܽ�������������ݶ�96�ֽڣ���C1��64�ֽڣ���C3��32�ֽڣ�����ǰ��0x00

	����ֵ��
			-1��        ����ʧ��
			msglen+96�� ���ܳɹ�
	*/

	int  sm2_decrypt(uint8_t* msg, int msglen, uint8_t* priKey, int priKeyLen, uint8_t* outmsg);
	/*
	���ܣ���SM2˽Կ�������ݡ����ܽ��������������96�ֽڣ�
	[����] msg     Ҫ���ܵ����ݣ���sm2_encrypt()���ܵĽ����������96�ֽڡ�
	[����] msglen��msg���ֽ���
	[����] priKey�� ˽Կ
	[����] priKeyLen�� priKeyLen���ֽ���

	[���] outmsg: ���ܽ����������������96�ֽڣ�

	����ֵ��
			-1��        ����ʧ��
			msglen-96�� ���ܳɹ�
	*/

	void sm2_kepubYgreement_a1_3(uint8_t* kx1, int* kx1len,
		uint8_t* ky1, int* ky1len,
		uint8_t* ra, int* ralen
	);

	/*
	���ܣ���ԿЭ�̵ķ��𷽵��ô˺�������һ����ʱ��Կ(kx1, ky1)����Ӧ�����������Կ���͸��Է��������ra�Լ����档
	[���] kx1��   ��Կ��X���꣬����32�ֽ���ǰ���0x00
	[���] kx1len��kx1���ֽ�����32
	[���] ky1��   ��Կ��Y���꣬����32�ֽ���ǰ���0x00
	[���] ky1len��ky1���ֽ�����32
	[���] ra:     �����������32�ֽ���ǰ���0x00
	[���] ralen�� ra���ֽ�����32

	����ֵ����

	*/

	#ifdef __cplusplus
		}
	#endif

#endif