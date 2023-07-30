#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include "sm2.h"
#include "tools.h"
using namespace std;
#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI ThreadFun(LPVOID lpThreadParameter);
SM2_KEY sm2Key;
SM2_PUBKEY CilentKey;
int Cilent();
int Server();




int main() {
	//Server();
	Cilent();
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
	T_INIT(10);

	//3. ����sm2��˽Կ�����͸������
	sm2_keygen(&sm2Key);
	memcpy(buf, &(sm2Key.pubkey), sizeof(sm2Key.pubkey));
	send(s, buf, 1024, 0);

	//4. ��ʱ������˷���Ϣ
	int  ret = 0;
	do {
		uint8_t buf[1024] = { 0 };
		uint8_t outbuf[1024] = { 0 };
		uint8_t signature[1024] = { 0 };
		uint8_t digest[1024] = { 0 };
		int textLen;
		cout << "\n���뷢����Ϣ:";
		//cin >> buf;
		cin.getline((char*)buf, 1024);
		textLen = strlen((const char*)buf);
		if (textLen == 0) {
			continue;
		}

		T_STR("ǩ��");
		sm3_e(NULL, 0, sm2Key.pubkey, buf, textLen, digest);
		sm2_sign(digest, 32, sm2Key.priKey, sm2Key.priKeyLen, signature, NULL, signature + 32, NULL);
		T_END("ǩ��", 0);


		memcpy(outbuf, &textLen, sizeof(int));
		memcpy(outbuf + sizeof(int), buf, textLen);
		memcpy(outbuf + sizeof(int) + textLen, signature, 64);

		ret = send(s, (const char*)outbuf, 1024, 0);

		printHex("ǩ��", signature, 64);
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

	//���չ�Կ
	char buf[1024] = { 0 };
	recv(c, (char*)buf, 1024, 0);
	memcpy(&CilentKey, buf, sizeof(CilentKey));

	//ѭ�����տͻ�������
	int ret = 0;

	T_INIT(10);

	do
	{
		uint8_t buf[1024] = { 0 };
		uint8_t text[1024] = { 0 };
		uint8_t signature[64] = { 0 };
		uint8_t digest[32] = { 0 };
		int textLen;

		ret = recv(c, (char*)buf, 1024, 0);
		memcpy(&textLen, buf, sizeof(int));
		memcpy(text, buf + sizeof(int), textLen);
		memcpy(signature, buf + sizeof(int) + textLen, 64);

		printHex_ASCII("����", text, textLen);

		T_STR("��֤");
		sm3_e(NULL, 0, CilentKey, text, textLen, digest);
		ret = sm2_verify(digest, 32, signature, 32, signature + 32, 32, CilentKey);
		T_END("��֤", 0);

		printHex("sm3Hash", digest, 32);
		printHex("sigCR", signature, 32);
		printHex("sigCS", signature + 32, 32);

		if (ret) {
			printf("\n��֤ͨ��\n");
		}
		else {
			printf("\n��֤ʧ��\n");
		}
		printf("\n\n\n\n");
	} while (ret != SOCKET_ERROR && ret != 0);

	cout << c << "�Ͽ�����" << endl;

	return 0;
}



