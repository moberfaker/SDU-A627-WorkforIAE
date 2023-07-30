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
	功能：根据用户ID及公钥，求用于签名或验签的消息HASH值
	[输入] userid： 用户ID
	[输入] userid_len： userid的字节数
	[输入] pubX： 公钥的X坐标
	[输入] pubXLen: pubX的字节数
	[输入] pubY： 公钥的Y坐标
	[输入] pubYLen: pubY的字节数
	[输入] msg：要签名的消息
	[输入] msg_len： msg的字节数
	[输出] e：32字节，用于签名或验签

	返回值：
			－1：内存不足
			  0：成功
	*/

	void sm2_keygen(SM2_KEY* key);
	/*
	功能：生成SM2公私钥对
	[输出] pubX：   公钥的X坐标，不足32字节在前面加0x00
	[输出] pubXLen: pubX的字节数，32
	[输出] pubY：   公钥的Y坐标，不足32字节在前面加0x00
	[输出] pubYLen: pubY的字节数，32
	[输出] priKey：私钥，不足32字节在前面加0x00
	[输出] priKeyLen： priKey的字节数，32
	*/

	void sm2_sign(uint8_t* hash, int hashlen, uint8_t* priKey, int priKeyLen, uint8_t* cr, int* rlen, uint8_t* cs, int* slen);
	/*
	功能：SM2签名
	[输入] hash：    sm3_e()的结果
	[输入] hashlen： hash的字节数，应为32
	[输入] priKey： 私钥
	[输入] priKeyLen： priKeyLen的字节数

	[输出] cr：  签名结果的第一部分，不足32字节在前面加0x00。
	[输出] rlen：cr的字节数，32
	[输出] cs：  签名结果的第二部分，不足32字节在前面加0x00。
	[输出] slen：cs的字节数，32
	*/

	int  sm2_verify(uint8_t* hash, int hashlen, uint8_t* cr, int rlen, uint8_t* cs, int slen, SM2_PUBKEY pk);
	/*
	功能：验证SM2签名
	[输入] hash：    sm3_e()的结果
	[输入] hashlen： hash的字节数，应为32
	[输入] cr：  签名结果的第一部分
	[输入] rlen：cr的字节数
	[输入] cs：  签名结果的第二部分。
	[输入] slen：cs的字节数
	[输入] pubX：   公钥的X坐标
	[输入] pubXLen: pubX的字节数，不超过32字节
	[输入] pubY：   公钥的Y坐标
	[输入] pubYLen: pubY的字节数，不超过32字节

	返回值：
			0：验证失败
			1：验证通过
	*/

	int  sm2_encrypt(uint8_t* msg, int msglen, SM2_PUBKEY* pubKey, uint8_t* outmsg);
	/*
	功能：用SM2公钥加密数据。加密结果比输入数据多96字节！
	[输入] msg     要加密的数据
	[输入] msglen：msg的字节数
	[输入] pubX：    公钥的X坐标
	[输入] pubXLen:  pubX的字节数，不超过32字节
	[输入] pubY：    公钥的Y坐标
	[输入] pubYLen:  pubY的字节数，不超过32字节

	[输出] outmsg: 加密结果，比输入数据多96字节！，C1（64字节）和C3（32字节）保留前导0x00

	返回值：
			-1：        加密失败
			msglen+96： 加密成功
	*/

	int  sm2_decrypt(uint8_t* msg, int msglen, uint8_t* priKey, int priKeyLen, uint8_t* outmsg);
	/*
	功能：用SM2私钥解密数据。解密结果比输入数据少96字节！
	[输入] msg     要解密的数据，是sm2_encrypt()加密的结果，不少于96字节。
	[输入] msglen：msg的字节数
	[输入] priKey： 私钥
	[输入] priKeyLen： priKeyLen的字节数

	[输出] outmsg: 解密结果，比输入数据少96字节！

	返回值：
			-1：        解密失败
			msglen-96： 解密成功
	*/

	void sm2_kepubYgreement_a1_3(uint8_t* kx1, int* kx1len,
		uint8_t* ky1, int* ky1len,
		uint8_t* ra, int* ralen
	);

	/*
	功能：密钥协商的发起方调用此函数产生一对临时公钥(kx1, ky1)和相应的随机数。公钥发送给对方，随机数ra自己保存。
	[输出] kx1：   公钥的X坐标，不足32字节在前面加0x00
	[输出] kx1len：kx1的字节数，32
	[输出] ky1：   公钥的Y坐标，不足32字节在前面加0x00
	[输出] ky1len：ky1的字节数，32
	[输出] ra:     随机数，不足32字节在前面加0x00
	[输出] ralen： ra的字节数，32

	返回值：无

	*/

	#ifdef __cplusplus
		}
	#endif

#endif