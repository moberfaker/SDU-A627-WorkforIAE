#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include "sm2.h"
using  namespace std;
#pragma comment(lib, "ws2_32.lib")

SM2_

int  main() {
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
	

	//3��ʱ������˷���Ϣ
	int  ret = 0;
	do {
		char buf[1024] = { 0 };
		cout << "���뷢����Ϣ:";
		cin >> buf;
		ret = send(s, buf, 1024, 0);
	} while (ret != SOCKET_ERROR && ret != 0);

	//4.�رռ����׽���
	closesocket(s);
	//����winsock2�Ļ���
	WSACleanup();
	return 0;
}

