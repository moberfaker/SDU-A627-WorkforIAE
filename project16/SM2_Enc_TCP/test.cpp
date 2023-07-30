#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include "sm2.h"
#include "tools.h"
using namespace std;
#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI ThreadFun(LPVOID lpThreadParameter);
SM2_KEY sm2Key;
SM2_PUBKEY ServerKey;
int Cilent();
int Server();

int main() {
	//Cilent();
	Server();
}

int Server() {
	//����winsock2�Ļ���
	WSADATA wd;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wd)) {
		cout << "WSAStartup error : " << GetLastError() << endl;
		return 0;
	}
	//1. ������ʽ�׽���
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == s) {
		cout << "socket error : " << GetLastError() << endl;
		return 0;
	}
	//2. �󶨶˿ں�Ip
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8891);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);//ip��ַת�����ֽ���
	int len = sizeof(sockaddr_in);
	if (SOCKET_ERROR == bind(s, (SOCKADDR*)&addr, len)) {
		cout << "bind error : " << GetLastError() << endl;
		return 0;
	}

	//3. ����sm2��˽Կ
	sm2_keygen(&sm2Key);

	//3. ����
	listen(s, 5);

	//���߳�ѭ�����տͻ��˵�����
	while (true) {
		sockaddr_in addrClient;
		len = sizeof(sockaddr_in);
		//4. ���ճɹ�������clientͨѶ��socket
		SOCKET c = accept(s, (SOCKADDR*)&addrClient, &len);
		if (INVALID_SOCKET != c) {
			//�����߳� ���Ҵ�����clientͨѶ���׽���
			HANDLE hThread = CreateThread(NULL, 0, ThreadFun, (LPVOID)c, 0, NULL);
			CloseHandle(hThread); //�رն��̵߳�����
		}
	}
	//6. �رռ����׽���
	closesocket(s);
	//����winsock2�Ļ���
	WSACleanup();

	return 0;
}

int Cilent() {
	//����winsock2�Ļ���
	WSADATA  wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		cout << "WSAStartup  error��" << GetLastError() << endl;
		return 0;
	}
	//1.������ʽ�׽���
	SOCKET  s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		cout << "socket  error��" << GetLastError() << endl;
		return 0;
	}
	//2.���ӷ�����
	sockaddr_in   addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8891);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);//ip��ַת�����ֽ���
	int len = sizeof(sockaddr_in);
	if (connect(s, (SOCKADDR*)&addr, len) == SOCKET_ERROR) {
		cout << "connect  error��" << GetLastError() << endl;
		return 0;
	}
	//3���շ���˵���Ϣ
	char buf[1024] = { 0 };
	recv(s, buf, 1024, 0);
	cout << buf << endl;

	//4��ȡ����˹�Կ
	recv(s, buf, 1024, 0);
	memcpy(&ServerKey, buf, sizeof(ServerKey));

	T_INIT(10);

	//3��ʱ������˷���Ϣ
	int  ret = 0;
	do {
		uint8_t buf[1024] = { 0 };
		uint8_t cipher[1024] = { 0 };
		int textLen;
		cout << "���뷢����Ϣ:";
		//cin >> buf;
		cin.getline((char*)buf, 1024);
		textLen = strlen((const char*)buf);
		if (textLen == 0) {
			continue;
		}

		T_STR("����");
		sm2_encrypt(buf, textLen, &ServerKey, cipher);
		T_END("����", 0);

		textLen += 96;

		memcpy(buf, &textLen, sizeof(int));
		memcpy(buf + sizeof(int), cipher, textLen);

		ret = send(s, (const char*)buf, 1024, 0);

		printHex("����", cipher, textLen);
		printf("\n\n\n\n");
	} while (ret != SOCKET_ERROR && ret != 0);

	//4.�رռ����׽���
	closesocket(s);
	//����winsock2�Ļ���
	WSACleanup();
	return 0;
}

DWORD WINAPI ThreadFun(LPVOID lpThreadParameter) {
	//5. ��ͻ���ͨѶ ���ͻ��߽�������
	SOCKET c = (SOCKET)lpThreadParameter;
	cout << c << "���ӳɹ�" << endl;

	//�������ݺ͹�Կ
	char buf[1024] = { 0 };
	sprintf_s(buf, "%d���ӳɹ�!", c);
	send(c, buf, 1024, 0);
	Sleep(1);
	memcpy(buf, &(sm2Key.pubkey), sizeof(sm2Key.pubkey));
	send(c, buf, 1024, 0);
	//ѭ�����տͻ�������
	int ret = 0;

	T_INIT(10);

	do
	{
		uint8_t buf[1024] = { 0 };
		uint8_t text[1024] = { 0 };
		int textLen;

		ret = recv(c, (char*)buf, 1024, 0);
		memcpy(&textLen, buf, sizeof(int));
		memcpy(text, buf + sizeof(int), textLen);


		printHex("����", text, textLen);

		T_STR("����");
		sm2_decrypt(text, textLen, sm2Key.priKey, sm2Key.priKeyLen, buf);
		T_END("����", 0);

		printHex_ASCII("����", buf, textLen - 96);
		printf("\n\n\n\n");
	} while (ret != SOCKET_ERROR && ret != 0);

	cout << c << "�Ͽ�����" << endl;

	return 0;
}

