#ifndef IAE_SM4_H
#define IAE_SM4_H

#include <stdint.h>

#define SM4_ENCRYPT 1
#define SM4_DECRYPT 0

#define SM4_blockS 16
#define SM4_ROUNDS_NUM 32

	#ifdef __cplusplus
	extern "C" {
	#endif

//SM4¬÷√ÿ‘ø
typedef struct {
	uint32_t rk[SM4_ROUNDS_NUM];
}SM4_RK;

void sm4_keyGen(const uint8_t* key, SM4_RK* ks);

void IAE_SM4_encrypt(const uint8_t* inbuf, uint8_t* outbuf, const SM4_RK* ks);

void IAE_SM4_decrypt(const uint8_t* inbuf, uint8_t* outbuf, const SM4_RK* ks);

void sm4Enc(const uint8_t* inbuf, size_t dataLen, uint8_t* outbuf, const uint8_t* key);

void sm4Dec(const uint8_t* inbuf, size_t dataLen, uint8_t* outbuf, const uint8_t* key);

	#ifdef __cplusplus
		}
	#endif

#endif