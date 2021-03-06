// Server.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//服务端：  
#include <iostream>  
#include <stdio.h>  
#include <windows.h>  //一定要包含该头文件

using namespace std;
#pragma comment(lib, "WS2_32.lib") 
//windwows下的socket编程函数库
int main()
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	//windows网络编程库的版本号信息
	SOCKET sListen = 0;  //TCP通信的socket数据结构
	sockaddr_in sin = { 0 };
	//创建一个socket编程类型的网络地址数据结构这个用于本地
	sockaddr_in remoteAddr = { 0 };
	//创建一个socket编程类型的网络地址数据结构这个用于储存远程主机的
	//IP地址和端口号 
	char szText[] = "TCP Server Demo";
	int nAddrLen = 0;
	nAddrLen = sizeof(sockaddr_in);
	//计算这个sockaddr_in数据结构的大小
	sin.sin_port = ntohs(55533);
	//设置本地（这里指服务端）计算机要打开的端口
	sin.sin_family = AF_INET;  //设置网络通信的网络协议族类型
	//sin.sin_addr.S_un.S_addr = INADDR_ANY;
	sin.sin_addr.S_un.S_addr = inet_addr("192.168.0.111");
	//设置本地计算机的IP地址，一般INADDR_ANY在程序运行时
	//会自动计算成本地的IP地址的
	//init wsa  
	if (WSAStartup(sockVersion, &wsaData) != 0)
		//WSAStartup函数是在程序中初始化并加载Windows网络
	{
		cout << "initlization failed!" << endl;
		exit(0);
		//如果WSAStartup返回值为1的话就表示ws2_32.dll文件有问题，程序退出
	}

	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//在本机上创建一个socket
	//使用bind函数绑定本机的ip和打开端口到本机创建的socket结构上，并初始化该socket
	//重点说明一个在服务器上是用bind函数来初始化socket，在客户机上是用connect函数来初始化socket的喔
	if (bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "bind failed!" << endl;
		return 0;  //如果socket绑定失败返回程序0并退出程序
	}
	if (listen(sListen, 2) == SOCKET_ERROR)
		//listen是用来打开本地计算机的端口，参数2表示客户端同
		//时连接服务器的数量，这里是说可以同时有2个客户端连接
		//到本机打开的端口
	{
		cout << "listen failed!" << endl;
		return 0;
		//如果打开本地端口失败就返回0并退出程序 
	}

	SOCKET sClient = INADDR_ANY;
	//再服务器上再创建一个socket结构，用来储存与一个客户端进行通信连
	//接的连接实例   
	sClient = accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
	if (sClient == INVALID_SOCKET)
	{
		cout << "accept failed!" << endl;
		getchar();
		return 0;
		//如果本地计算机（服务器）接受一个客户端连接请求失败就退出监听状态
	}
	char buffer[256] = "\0";  //声明一个从网络接收数据的缓存区
	int  nRecv = 0;
	while (true)
	{
		nRecv = recv(sClient, buffer, 256, 0);
		if (nRecv > 0)
		{
			buffer[nRecv] = '\0';
			//如果接收到网络数据包长度大于0的话就在接收到的数据包未尾添加一个字符串
			//结束符  
			cout << "reveive data: " << buffer << endl;
			//按字符串格式输出接收到的数据 
		}

		//accept函数是用来创建一个TCP通信连接实例的，他使用一个远程计算机的IP地址和本地计算机上创建的一个
		//socket，这两个信息来创建一个本地计算机到远程计算机的一个TCP通信连接实例的socket，sClient就是当前
		//程序中的这个连接实例的socket 了。注accept函数也可以直接理解为接受一个客户机的连接请求。     
		//sClient = accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
		//if (sClient == INVALID_SOCKET)
		//{
		//	cout << "accept failed!" << endl;
		//	continue;
			//如果本地计算机（服务器）接受一个客户端连接请求失败就退出监听状态
		//}
		//send函数是在TCP连接建立之后，就来发送数据的。其中sClient是存放有一个连接实例的socket结构来
		//的。这个服务器上的send函数的socket参数和客户机上的socket参数是不同的喔，记住这里的socket参数是通过
		//accept函数创建的喔。不是用connet函数和bind函数创建喔。一定要记住这个sClient是一个关键来的。然后
		//szText就是我们想要发送的数据了。这里我们向连接到该服务器的客户端发送了一个“TCP Server Demo”的信息。
		cin >> szText;
		send(sClient, szText, strlen(szText), 0);
		
		//关闭这个用accept函数建立socket对象，关闭与remoteAddr连接通信。
		//remoteAddr这结构理包含了有远程计算机的IP和端口号信息，当客户机
		//使用connect函数发送一个连接请求时，被accept函数接受并处理后，远
		//程计算机的IP地址和端口信息就被填到了remoteAddr这个结构中去

	}
	closesocket(sClient);
	closesocket(sListen);
	//关闭本地计算机（服务器）上的由bind函数创建的socket对象，关闭本机
	//打开的端口，关闭服务器  
	WSACleanup();
	getchar();
	getchar();
	//释放ws2_32.dll动态库
}

