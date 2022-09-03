#pragma once
#include "SocketEvent.h"

class SocketUtil
{
public:
    static LPFN_DISCONNECTEX m_gDisconnect;

public:
    static void         InitWinSock();
    static SOCKET       SocketCreate();
    static void         SocketClose(SOCKET _socket);
    static void         SocketEventHandle(HANDLE _IOCPhandle);
};