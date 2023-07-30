#include "tools.h"

void Paddata(uint8_t** data, size_t* dataLen, int padLen) {
	if (*dataLen % padLen == 0) {
		return;
	}
	else {
		size_t newLen = *dataLen + (padLen - *dataLen % padLen);
		uint8_t* temp = (uint8_t*)realloc(*data, newLen);
		if (temp == NULL) {
			printf("Realloc Failed for padding!");
			exit(-1);
		}
		else {
			*dataLen = newLen;
			*data = temp;
		}
	}
}

void printHex(const char* s, const uint8_t* data, size_t len)
{
	printf("\n%s: \n\t\t", s);
	for (size_t i = 0; i < len; i++) {
		if (i != 0 && i % BLOCK_BYTE == 0)printf(" ");
		if (i != 0 && i % LINE_BYTE == 0)
			printf("\n\t\t");
		printf("%02X", data[i]);
	}
	printf("\n");
}

void printHex_ASCII(const char* s, const uint8_t* data, size_t len)
{
	int left = len % LINE_BYTE;
	int temp = len - left;

	printf("\n%s: \n\t\t", s);
	if (temp != 0) {
		for (size_t i = 0; i <= temp; i++) {
			if (i != 0 && i % BLOCK_BYTE == 0)printf(" ");
			if (i != 0 && i % LINE_BYTE == 0) {
				printf(" ");
				for (int j = i - LINE_BYTE; j < i; j++) {
					if (data[j] >= 32 && data[j] <= 127)
						printf("%c", data[j]);
					else
						printf(".");
				}
				printf("\n\t\t");
				if (i == temp)break;
			}
			printf("%02X", data[i]);
		}
	}
		

	for (size_t i = temp; i < len; i++) {
		if (i != temp && i % BLOCK_BYTE == 0)printf(" ");
		printf("%02X", data[i]);
	}

	left = (left >= LINE_BYTE) ? LINE_BYTE * 2 + 2 - left * 2 : LINE_BYTE * 2 + 3 - left * 2;
	printf("%*s", left, "");
	for (size_t i = temp; i < len; i++) {
		if (data[i] >= 32 && data[i] <= 127)
			printf("%c", data[i]);
		else
			printf(".");
	}
	if (len % LINE_BYTE)printf("\n");
}