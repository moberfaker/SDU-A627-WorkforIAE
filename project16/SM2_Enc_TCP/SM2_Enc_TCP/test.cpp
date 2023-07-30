#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include "sm2.h"
using  namespace std;
#pragma comment(lib, "ws2_32.lib")

SM2_

int  main() {
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
	

	//3随时给服务端发消息
	int  ret = 0;
	do {
		char buf[1024] = { 0 };
		cout << "输入发送消息:";
		cin >> buf;
		ret = send(s, buf, 1024, 0);
	} while (ret != SOCKET_ERROR && ret != 0);

	//4.关闭监听套接字
	closesocket(s);
	//清理winsock2的环境
	WSACleanup();
	return 0;
}

