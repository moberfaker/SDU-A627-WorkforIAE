#include "sm4.h"
#include <malloc.h>
#include <stdio.h>

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

	SM4_RK ks;
	sm4_keyGen(key, &ks);

	IAE_SM4_encrypt(text, output, &ks);
	printHex("cipher(Enc)", output, 16);

	IAE_SM4_decrypt(output, output, &ks);
	printHex("text(Dec)", output, 16);
	return 0;
}