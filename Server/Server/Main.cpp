﻿#include "pch.h"
#define LOG(text) cout << text << endl;
//
//enum IO_TYPE
//{
//    READ,
//    WRITE,
//    ACCEPT,
//    CONNECT,
//};
//
//struct OverlappedEx
//{
//    WSAOVERLAPPED overlapped = {};
//    int type = 0; // read, write, accept, connect ...
//};
//

void Broadcast(char _sendbuffer[BUFSIZE])
{
    std::vector<Session*> vecSession = g_SessionManager->GetSessions();

    int nCount = (int)vecSession.size();
    for (int i = 0; i < nCount; ++i)
    {
        SocketEvent* sEvent = new SocketEvent(SocketEventType_Send, vecSession[i]);

        WSABUF wsaBuf;
        wsaBuf.buf = _sendbuffer;
        wsaBuf.len = BUFSIZE;

        DWORD sendLen = 0;
        DWORD flags = 0;

        WSASend(vecSession[i]->GetSocket(), &wsaBuf, 1, &sendLen, flags, (LPWSAOVERLAPPED)sEvent, NULL);
    }
}
void SocketEventHandle(HANDLE _IOCPhandle)
{
    while (true)
    {
        DWORD bytesTransferred = 0;
        ULONG_PTR key = 0;
        SocketEvent* socketEvent = nullptr;

        // 대기열 완료 현황
        // 

        BOOL ret = GetQueuedCompletionStatus(_IOCPhandle, &bytesTransferred,
            OUT &key, (LPOVERLAPPED*)&socketEvent, INFINITE);


        if (ret == FALSE || bytesTransferred == 0)
        {
            // TODO : 연결 끊김
            continue;
        }

        Session* session = socketEvent->GetSession();

        if(socketEvent->GetSocketEventType() == SocketEventType_Receive)
        {
            Broadcast(session->m_recvBuffer);

            SocketEvent* sEvent = new SocketEvent(SocketEventType_Receive, session);

            DWORD recvLen = 0;
            DWORD flags = 0;

            cout << session->GetSessionNumber() << " : " << session->m_recvBuffer << endl;

            ::WSARecv(session->GetSocket(), session->GetInitRecvieWSABUF(), 1, &recvLen, &flags, (LPWSAOVERLAPPED)sEvent, NULL);
        }
        else if (socketEvent->GetSocketEventType() == SocketEventType_Send)
        {
            cout << "SocketEventType_Send" << endl;
        }

        delete socketEvent;
    }
}


int main()
{
    LOG("Server Start!!!");
    /*
    1. 윈도우 소켓 생성
    2. 소켓 생성(서버)
    3. 소켓 주소 생성(서버)
    4. Bind함수 호출
    5. Listen함수 호출
    6. CreateIoCompletionPort함수 호출

    // 클라가 접속했을경우
    7. Accept함수 호출
    8. CreateIoCompletionPort함수 호출
    9. WSARecv함수 호출
    */

    // Select
    // WSAEvent

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
    SOCKET listenSocket = SocketUtil::SocketCreate();

    SOCKADDR_IN serverAddr;
    {
        // SOCKADDR_IN
        // 소켓의 주소를 표현하는 구조체다.
        // 범용 주소 구조체인 SOCKADDR를 사용하지만 주소체계에 따라 전용 구조체를 만든다
        // SOCKADDR_IN은 IPv4 주소체계 전용으로 사용하기 위한 구조체다.
        
        memset(&serverAddr, 0, sizeof(serverAddr));

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
        serverAddr.sin_port = ::htons(7777);
    }

    {
        // bind 함수 는 소켓 주소 표현 구조체와 소켓을 연결하기위해 사용하는것
        if (::bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        {
            LOG("bind Fail");
            return 0;
        }
    }

    {
        // 다른 프로세스?(ex.클라이언트)에서 연결요청을 했을경우 처리가 가능한 상태
        //
        
        if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
        {
            LOG("listen Fail");
            return 0;
        }
    }

    // CreateIoCompletionPort함수는 2가지 용도로 사용한다.
    // 1. IOCP커널 오브젝트를 생성하기위해 사용된다.
    //    처음에 한번 호출을 해줘야한다.
    //    호출후에는 IOCP 핸들을 반환한다.
    // 2. IOCP 완료 포트 핸들과 소켓 핸들을 연결하면 프로세스에서 
    //    해당 소켓 핸들과 관련된 비동기 I/O 작업의 완료 알림을 받을수 있다.

    HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);


    // IOCP를 받을 쓰레드를 돌린다.
    g_ThreadManager->Run([&iocpHandle]
    {
        SocketEventHandle(iocpHandle);
    });

    // 여기서 클라이언트에서 연결을 받는다.
    while (true)
    {
        // 클라이언트 소켓 주소
        SOCKADDR_IN clinetAddr;
        int addrLen = sizeof(clinetAddr);

        // 외부에서 연결시도를 했을경우 시도한 해당 소켓을 뱉어준다.
        SOCKET clientSocket = ::accept(listenSocket, (sockaddr*)&clinetAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET)
            return 0;

        

        Session* session = g_SessionManager->CreateSession();
        session->SetSocket(clientSocket);


        cout << session->GetSessionNumber() << " Join!!!" << endl;

        // 2번의 형식
        // IOCP 완료 포트 핸들과 소켓 핸들을 연결하면 프로세스에서 해당 소켓 핸들과 관련된 비동기 I/O 작업의 완료 알림을 받을수 있다.
        // 핸들(소켓)을 iocp와 연결
        CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle, (ULONG_PTR)session, 0);

        {
            // 버퍼의 주소와 버퍼의 길이를 저장
            // 주소와 길이를 저장하는이유
            // WSABUF 배열 형식으로 한번에 전달 가능하게끔 하기위해
            // 버퍼를 모아서 한번에 출력하는기법이 Scatter-Gather
            WSABUF wsaBuf;
            wsaBuf.buf = session->m_recvBuffer;
            wsaBuf.len = BUFSIZE;

            SocketEvent* sEvent = new SocketEvent(SocketEventType_Receive, session);

            DWORD recvLen = 0;
            DWORD flags = 0;

            WSARecv(session->GetSocket(), session->GetInitRecvieWSABUF(), 1, &recvLen, &flags, (LPWSAOVERLAPPED)sEvent, NULL);
        }
    }
    
    // 윈도우 소켓 해제
    WSACleanup();
}