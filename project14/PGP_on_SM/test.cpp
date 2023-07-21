#include "sm4.h"
#include "sm3.h"
#include "sm2.h"
#include "PGP.h"
#include "tools.h"
#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

void TestSafty() {
	PGP alice("Alice");
	PGP bob("Bob");
	PGP tom("Tom");

	//双方各自生成sm2公私钥
	alice.BuildKey();
	bob.BuildKey();
	tom.BuildKey();

	//alice向bob发送“Hi Bob,I am Alice!”
	cout << "安全性测试：alice向bob发送消息" << endl;
	uint8_t message[] = "Hi Bob,I am Alice!";
	alice.Setmessage(message, sizeof(message));	//设置信息
	alice.SetRecverPubKey(bob.GetPubKey());		//指定收件人
	alice.SendData();	//生成保密信息

	printHex_ASCII("alice message", alice.message, alice.messageLen);
	printHex_ASCII("alice PGPmessage", alice.PGPmessage, alice.PGPmessageLen);

	//bob读取PGP加密后的信息
	bob.ReadData(alice.PGPmessage, alice.PGPmessageLen);
	bob.ShowData();

	//tom读取PGP加密后的信息
	tom.ReadData(alice.PGPmessage, alice.PGPmessageLen);
	tom.ShowData();
}

#define TEST_TIME 100
void TestSpeed() {

	cout << "速度/吞吐量测试：alice向bob发送1024字节消息" << endl;
	T_INIT(TEST_TIME);

	PGP alice("Alice");
	PGP bob("Bob");

	T_STR("Key_Gen");
	alice.BuildKey();
	T_END("Key_Gen",0);
	bob.BuildKey();

	//测试1024Bytes的数据量
	uint8_t* message = (uint8_t*)malloc(1024);
	memset(message, 0x55, 1024);

	alice.Setmessage(message, sizeof(message));	//设置信息
	alice.SetRecverPubKey(bob.GetPubKey());		//指定收件人

	T_STR("PGPData_Gen");
	alice.SendData();	//生成保密信息
	T_END("PGPData_Gen", 1024);

	T_STR("PGPData_Read");
	bob.ReadData(alice.PGPmessage, alice.PGPmessageLen);
	T_END("PGPData_Read",1024);
}

int main() {
	TestSafty();
	printf("\n\n");
	TestSpeed();
	system("pause");
	return 0;
}