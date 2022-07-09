#pragma once
#include "SocketEvent.h"

class SocketUtil
{
public:
	static SOCKET SocketCreate()
	{
		return ::socket(AF_INET, SOCK_STREAM, 0);
	}

	static void SocketEventHandle(HANDLE _IOCPhandle)
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

            Session* session = socketEvent->GetSession();

            socketEvent->SocketEventHandling(bytesTransferred);

            delete socketEvent;
        }
	}
};