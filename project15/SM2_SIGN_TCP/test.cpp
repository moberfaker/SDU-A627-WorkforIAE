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
	T_INIT(10);

	//3. 创建sm2公私钥并发送给服务端
	sm2_keygen(&sm2Key);
	memcpy(buf, &(sm2Key.pubkey), sizeof(sm2Key.pubkey));
	send(s, buf, 1024, 0);

	//4. 随时给服务端发消息
	int  ret = 0;
	do {
		uint8_t buf[1024] = { 0 };
		uint8_t outbuf[1024] = { 0 };
		uint8_t signature[1024] = { 0 };
		uint8_t digest[1024] = { 0 };
		int textLen;
		cout << "\n输入发送消息:";
		//cin >> buf;
		cin.getline((char*)buf, 1024);
		textLen = strlen((const char*)buf);
		if (textLen == 0) {
			continue;
		}

		T_STR("签名");
		sm3_e(NULL, 0, sm2Key.pubkey, buf, textLen, digest);
		sm2_sign(digest, 32, sm2Key.priKey, sm2Key.priKeyLen, signature, NULL, signature + 32, NULL);
		T_END("签名", 0);


		memcpy(outbuf, &textLen, sizeof(int));
		memcpy(outbuf + sizeof(int), buf, textLen);
		memcpy(outbuf + sizeof(int) + textLen, signature, 64);

		ret = send(s, (const char*)outbuf, 1024, 0);

		printHex("签名", signature, 64);
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

	//接收公钥
	char buf[1024] = { 0 };
	recv(c, (char*)buf, 1024, 0);
	memcpy(&CilentKey, buf, sizeof(CilentKey));

	//循环接收客户端数据
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

		printHex_ASCII("明文", text, textLen);

		T_STR("验证");
		sm3_e(NULL, 0, CilentKey, text, textLen, digest);
		ret = sm2_verify(digest, 32, signature, 32, signature + 32, 32, CilentKey);
		T_END("验证", 0);

		printHex("sm3Hash", digest, 32);
		printHex("sigCR", signature, 32);
		printHex("sigCS", signature + 32, 32);

		if (ret) {
			printf("\n验证通过\n");
		}
		else {
			printf("\n验证失败\n");
		}
		printf("\n\n\n\n");
	} while (ret != SOCKET_ERROR && ret != 0);

	cout << c << "断开链接" << endl;

	return 0;
}



