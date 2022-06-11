#pragma once


const int BUFSIZE = 1000;

class Session : WSAOVERLAPPED
{
public:
    Session();

public:
    SOCKET GetSocket() { return m_Socket; }
    void SetSocket(SOCKET _socket) { m_Socket = _socket; }

public:
    WSABUF* GetInitRecvieWSABUF();

    WSABUF* GetInitSendWSABUF();

    string GetSessionNumber();

    void SendPacketHandling();
    void ReceivePacketHandling();

public:
    char m_recvBuffer[BUFSIZE];
    char m_sendBuffer[BUFSIZE];

protected:
    SOCKET m_Socket = INVALID_SOCKET;
    WSABUF* m_wsaRecvieBuf;
    WSABUF* m_wsaSendBuf;
    int m_iSessionNumber = 0;

    /*
    SOCKET socket = INVALID_SOCKET;
    char recvBuffer[BUFSIZE] = {};
    int recvBytes = 0;*/
};

