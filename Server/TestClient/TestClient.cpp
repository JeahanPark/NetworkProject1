#include "pch.h"


/////////// 네트워크 관련
using namespace std;

int main()
{
    // 클라이언트는 일단 기본적인 걸로한다.


    cout << "Test Client!!!" << endl;
    // 윈도우 소켓 생성
    {
        WORD wVersionRequested;
        WSAData wsaData;

        wVersionRequested = MAKEWORD(2, 2);

        if (::WSAStartup(wVersionRequested, &wsaData) != 0)
        {
            cout << "WSAData Fail" << endl;
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

    HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    g_ThreadManager->Run([&iocpHandle]
    {
        SocketUtil::SocketEventHandle(iocpHandle);
    });

    ClientSession* session = g_SessionManager->CreateSession<ClientSession>();
    session->InitSession(iocpHandle, clientSocket);
    //std::thread t2(Receive, clientSocket);

  /*  t1.join();
    t2.join();*/

    while (true)
    {
        session->Chetting();
        //모르겠다 일단돌려
        Sleep(1000);
    }

    // 윈도우 소켓 해제
    WSACleanup();
}