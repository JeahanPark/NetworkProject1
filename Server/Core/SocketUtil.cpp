#include "pch.h"
#include "SocketUtil.h"

LPFN_DISCONNECTEX SocketUtil::m_gDisconnect = nullptr;

void SocketUtil::InitDisconnect()
{
    SOCKET socket = SocketCreate();
    
    DWORD bytes = 0;
    LPVOID* fn = (LPVOID*)m_gDisconnect;
    GUID guid = WSAID_DISCONNECTEX;
    int error = WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);

    SocketClose(socket);

    if(error ==  SOCKET_ERROR)
        CRASH("InitDisconnect");
}

void SocketUtil::SocketClose(SOCKET _socket)
{
    closesocket(_socket);
}

SOCKET SocketUtil::SocketCreate()
{
	return ::socket(AF_INET, SOCK_STREAM, 0);
}

void SocketUtil::SocketEventHandle(HANDLE _IOCPhandle)
{
    while (true)
    {
        DWORD bytesTransferred = 0;
        ULONG_PTR key = 0;
        SocketEvent* socketEvent = nullptr;

        // 대기열 완료 현황
        // 

        BOOL ret = GetQueuedCompletionStatus(_IOCPhandle, &bytesTransferred,
            OUT & key, (LPOVERLAPPED*)&socketEvent, INFINITE);


        if (ret == FALSE || bytesTransferred == 0)
        {
            // TODO : 연결 끊김
            continue;
        }

        shared_session session = socketEvent->GetSession();

        socketEvent->SocketEventHandling(bytesTransferred);

        delete socketEvent;
    }
}
