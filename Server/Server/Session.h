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
    void SendChatting(char chattingContent[]);


public:
    WSABUF* GetRecvieWSABUF();

    WSABUF* GetInitSendWSABUF();

    string GetSessionNumber();

    void SendPacketHandling();
    void ReceivePacketHandling(DWORD _bytesTransferred);


private:
    int GetRecvUseBuffer()
    {
        return m_iWritePos - m_iReadPos;
    }

private:
    // Recv
    char        m_recvBuffer[BUFSIZE];
    WSABUF*     m_wsaRecvieBuf;
    int         m_iReadPos = 0;
    int         m_iWritePos = 0;


    // Send
    char        m_sendBuffer[BUFSIZE];
    WSABUF*     m_wsaSendBuf;

private:
    SOCKET m_Socket = INVALID_SOCKET;


    int m_iSessionNumber = 0;

    /*
    SOCKET socket = INVALID_SOCKET;
    char recvBuffer[BUFSIZE] = {};
    int recvBytes = 0;*/
};

