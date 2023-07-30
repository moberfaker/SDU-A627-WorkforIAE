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
	//加载winsock2的环境
	WSADATA wd;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wd)) {
		cout << "WSAStartup error : " << GetLastError() << endl;
		return 0;
	}
	//1. 创建流式套接字
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == s) {
		cout << "socket error : " << GetLastError() << endl;
		return 0;
	}
	//2. 绑定端口和Ip
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8891);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);//ip地址转网络字节序
	int len = sizeof(sockaddr_in);
	if (SOCKET_ERROR == bind(s, (SOCKADDR*)&addr, len)) {
		cout << "bind error : " << GetLastError() << endl;
		return 0;
	}

	//3. 创建sm2公私钥
	sm2_keygen(&sm2Key);

	//3. 监听
	listen(s, 5);

	//主线程循环接收客户端的链接
	while (true) {
		sockaddr_in addrClient;
		len = sizeof(sockaddr_in);
		//4. 接收成功返回与client通讯的socket
		SOCKET c = accept(s, (SOCKADDR*)&addrClient, &len);
		if (INVALID_SOCKET != c) {
			//创建线程 并且传入与client通讯的套接字
			HANDLE hThread = CreateThread(NULL, 0, ThreadFun, (LPVOID)c, 0, NULL);
			CloseHandle(hThread); //关闭对线程的引用
		}
	}
	//6. 关闭监听套接字
	closesocket(s);
	//清理winsock2的环境
	WSACleanup();

	return 0;
}

int Cilent() {
	//加载winsock2的环境
	WSADATA  wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		cout << "WSAStartup  error：" << GetLastError() << endl;
		return 0;
	}
	//1.创建流式套接字
	SOCKET  s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		cout << "socket  error：" << GetLastError() << endl;
		return 0;
	}
	//2.链接服务器
	sockaddr_in   addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8891);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);//ip地址转网络字节序
	int len = sizeof(sockaddr_in);
	if (connect(s, (SOCKADDR*)&addr, len) == SOCKET_ERROR) {
		cout << "connect  error：" << GetLastError() << endl;
		return 0;
	}
	//3接收服务端的消息
	char buf[1024] = { 0 };
	recv(s, buf, 1024, 0);
	cout << buf << endl;

	//4获取服务端公钥
	recv(s, buf, 1024, 0);
	memcpy(&ServerKey, buf, sizeof(ServerKey));

	T_INIT(10);

	//3随时给服务端发消息
	int  ret = 0;
	do {
		uint8_t buf[1024] = { 0 };
		uint8_t cipher[1024] = { 0 };
		int textLen;
		cout << "输入发送消息:";
		//cin >> buf;
		cin.getline((char*)buf, 1024);
		textLen = strlen((const char*)buf);
		if (textLen == 0) {
			continue;
		}

		T_STR("加密");
		sm2_encrypt(buf, textLen, &ServerKey, cipher);
		T_END("加密", 0);

		textLen += 96;

		memcpy(buf, &textLen, sizeof(int));
		memcpy(buf + sizeof(int), cipher, textLen);

		ret = send(s, (const char*)buf, 1024, 0);

		printHex("密文", cipher, textLen);
		printf("\n\n\n\n");
	} while (ret != SOCKET_ERROR && ret != 0);

	//4.关闭监听套接字
	closesocket(s);
	//清理winsock2的环境
	WSACleanup();
	return 0;
}

DWORD WINAPI ThreadFun(LPVOID lpThreadParameter) {
	//5. 与客户端通讯 发送或者接收数据
	SOCKET c = (SOCKET)lpThreadParameter;
	cout << c << "链接成功" << endl;

	//发送数据和公钥
	char buf[1024] = { 0 };
	sprintf_s(buf, "%d链接成功!", c);
	send(c, buf, 1024, 0);
	Sleep(1);
	memcpy(buf, &(sm2Key.pubkey), sizeof(sm2Key.pubkey));
	send(c, buf, 1024, 0);
	//循环接收客户端数据
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


		printHex("密文", text, textLen);

		T_STR("解密");
		sm2_decrypt(text, textLen, sm2Key.priKey, sm2Key.priKeyLen, buf);
		T_END("解密", 0);

		printHex_ASCII("解密", buf, textLen - 96);
		printf("\n\n\n\n");
	} while (ret != SOCKET_ERROR && ret != 0);

	cout << c << "断开链接" << endl;

	return 0;
}

