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

	//˫����������sm2��˽Կ
	alice.BuildKey();
	bob.BuildKey();
	tom.BuildKey();

	//alice��bob���͡�Hi Bob,I am Alice!��
	cout << "��ȫ�Բ��ԣ�alice��bob������Ϣ" << endl;
	uint8_t message[] = "Hi Bob,I am Alice!";
	alice.Setmessage(message, sizeof(message));	//������Ϣ
	alice.SetRecverPubKey(bob.GetPubKey());		//ָ���ռ���
	alice.SendData();	//���ɱ�����Ϣ

	printHex_ASCII("alice message", alice.message, alice.messageLen);
	printHex_ASCII("alice PGPmessage", alice.PGPmessage, alice.PGPmessageLen);

	//bob��ȡPGP���ܺ����Ϣ
	bob.ReadData(alice.PGPmessage, alice.PGPmessageLen);
	bob.ShowData();

	//tom��ȡPGP���ܺ����Ϣ
	tom.ReadData(alice.PGPmessage, alice.PGPmessageLen);
	tom.ShowData();
}

#define TEST_TIME 100
void TestSpeed() {

	cout << "�ٶ�/���������ԣ�alice��bob����1024�ֽ���Ϣ" << endl;
	T_INIT(TEST_TIME);

	PGP alice("Alice");
	PGP bob("Bob");

	T_STR("Key_Gen");
	alice.BuildKey();
	T_END("Key_Gen",0);
	bob.BuildKey();

	//����1024Bytes��������
	uint8_t* message = (uint8_t*)malloc(1024);
	memset(message, 0x55, 1024);

	alice.Setmessage(message, sizeof(message));	//������Ϣ
	alice.SetRecverPubKey(bob.GetPubKey());		//ָ���ռ���

	T_STR("PGPData_Gen");
	alice.SendData();	//���ɱ�����Ϣ
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