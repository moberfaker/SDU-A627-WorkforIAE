#include <iostream>
#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
using namespace std;
//定义My_SM3长度
#define SM3_DIGEST_LENGTH 32 

struct tmp {
	unsigned char dgst[SM3_DIGEST_LENGTH]{};
};

void My_sm3(const unsigned char* msg, size_t msglen, unsigned char dgst[SM3_DIGEST_LENGTH]) {
	uint32_t _size = 32;
	EVP_Digest(msg, msglen, dgst, &_size, EVP_sm3(), NULL);
}


int main()
{
	unsigned char msg[] = "2021004600";
	size_t attack_len = 4;
	size_t Rho_len = 10;
	tmp* Rho = new tmp[Rho_len];
	My_sm3(msg, 26, Rho[0].dgst);
	int i = 0;
	clock_t t1 = clock();
	while (memcmp(Rho[i].dgst, Rho[(i + 1) % Rho_len].dgst, attack_len) != 0) {
		My_sm3(Rho[i].dgst, SM3_DIGEST_LENGTH , Rho[(i + 1) % Rho_len].dgst);
		i = (i + 1) % Rho_len;
	}
	clock_t t2 = clock();
	cout << "成功找到一个" << attack_len *8 << "比特长度的 rho 碰撞";
	cout << "时间开销为" << double(t2 - t1) / CLOCKS_PER_SEC <<"s" << endl;
	i = (i + 1) % Rho_len;
	for (int j = 0; j < Rho_len; j++) {
		for (int k = 0; k < SM3_DIGEST_LENGTH ; k++)
			printf("%02x", Rho[i].dgst[k]);
		printf("\n");
		i = (i + 1) % Rho_len;
	}
	delete[]Rho;
	return 0;
}