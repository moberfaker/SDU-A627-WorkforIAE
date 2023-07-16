#include "sm4.h"
#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define T_STR(lable) \
	printf("\n======== %s ========\n",lable);	\
	start = clock();						

#define T_END(lable,times) \
	time = clock() - start;		  \
	printf("耗时: %.2f ns\n", time*1000000/times);\
	printf("吞吐: %.4f Gbps\n",(((double)(times*16*8)/(1024*1024*1024))/time)*1000);	\
	printf("======== %s ========\n", lable);					

void printHex(const char* s, const uint8_t* data, size_t len)
{
	printf("%s: \t", s);
	for (size_t i = 0; i < len; i++) {
		if (i != 0 && i % 4 == 0)printf(" ");
		if (i != 0 && i % 16 == 0)printf("\n\t\t");
		printf("%02X", data[i]);
	}
	printf("\n");
}

int main() {
	uint8_t* key = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x09\x0A\x0B\x0C\x0D\x0E\x0F";
	uint8_t* text = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x09\x0A\x0B\x0C\x0D\x0E\x0F";
	uint8_t* output = malloc(16);
	printHex("key", key, 16);
	printHex("text", text, 16);

	clock_t start,end;
	int checkTime = 10000000;
	int i;
	double time;

	printf("\nTestTime: %d\n", checkTime);
	//秘钥生成
	T_STR("SM4 Gen Key");
	SM4_RK ks;
#pragma omp parallel for
	for (i = 0; i < checkTime; i++) {
		sm4_keyGen(key, &ks);
	}
	T_END("SM4 Gen Key", checkTime);

	//加密
	T_STR("SM4 Enc");
#pragma omp parallel for
	for (i = 0; i < checkTime; i++) {
		IAE_SM4_encrypt(text, output, &ks);
	}
	T_END("SM4 Enc", checkTime);
	printHex("cipher(Enc)", output, 16);

	//解密
	uint8_t* output2 = malloc(16);
	T_STR("SM4 Dec");
#pragma omp parallel for
	for (i = 0; i < checkTime; i++) {
		IAE_SM4_decrypt(output, output2, &ks);
	}
	T_END("SM4 Dec", checkTime);
	printHex("text(Dec)", output2, 16);

	free(output);
	free(output2);
	return 0;
}