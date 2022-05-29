#include <iostream>
#include <stdio.h>


/////////// 네트워크 관련
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
/////////// 네트워크 관련
using namespace std;

#define LOG(text) cout << text << endl;

int main()
{
    // 클라이언트는 일단 기본적인 걸로한다.


    LOG("Test Client!!!");
    // 윈도우 소켓 생성
    {
        WORD wVersionRequested;
        WSAData wsaData;

        wVersionRequested = MAKEWORD(2, 2);

        if (::WSAStartup(wVersionRequested, &wsaData) != 0)
        {
            LOG("WSAData Fail");
            return 0;
        }
    }

    //소켓 클래스는 네트워크 통신을 하기 위해 필요한 함수나 변수를 제공해주는 클래스입니다.
    SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        int errCode = ::WSAGetLastError();
        cout << "Socket ErrorCode : " << errCode << endl;
        return 0;
    }

    // 서버의 주소
    SOCKADDR_IN serverAddr;
    {
        // SOCKADDR_IN
        // 소켓의 주소를 표현하는 구조체다.
        // 범용 주소 구조체인 SOCKADDR를 사용하지만 주소체계에 따라 전용 구조체를 만든다
        // SOCKADDR_IN은 IPv4 주소체계 전용으로 사용하기 위한 구조체다.

        memset(&serverAddr, 0, sizeof(serverAddr));

        serverAddr.sin_family = AF_INET;
        ::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
        serverAddr.sin_port = ::htons(7777);
    }

    // 소켓에 서버의 소켓주소를 접속 해당 소켓은 바인딩된것으로 표시됌? 문서에 그렇게 써있음
    if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        int errCode = ::WSAGetLastError();
        cout << "Connect ErrorCode : " << errCode << endl;
        return 0;
    }

    cout << "Connected To Server!" << endl;

    while (true)
    {
        char sendBuffer[100] = "Hello World";

        // send는 연결된 소켓으로 데이터를 보낸다.
        // clientSocket는 소켓주소가 서버이므로 서버로 해당 버퍼를 보낸다.
        {
            int resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
            if (resultCode == SOCKET_ERROR)
            {
                int errCode = ::WSAGetLastError();
                cout << "Send ErrorCode : " << errCode << endl;
                return 0;
            }
            else
            {
                cout << "Client : " << sendBuffer << endl;
            }
        }

        // 1초 대기
        Sleep(1000);
    }

    // 윈도우 소켓 해제
    WSACleanup();
}
