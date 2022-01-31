#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	// 윈속 초기화
	// 이거를 따로 사용하지않음 window Socket을 쓸려면 이걸 초기화해줘야한다.
	// 첫번째 인자는 버전 2.2버전을 쓴다.
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	// ad : address Family (FA_INET = IPv4, AF_INET6 = IPv6)
	// type : TCP, UDP
	// protocol : 0
	// return : descriptor
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{

	}

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	// 데이터 초기화

	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	::WSACleanup();
}