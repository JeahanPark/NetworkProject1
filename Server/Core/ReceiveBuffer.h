#pragma once

class ReceiveBuffer
{
public:
    ReceiveBuffer();


public:

    WSABUF*      GetWSABuf();
    void        AllReadCheck();
    void        WritePosMove(DWORD _bytesTransferred);
    void        ReadPosMove(DWORD _bytesTransferred);
    int         GetRecvUseBuffer();
    char*       PacketAdress();




private:
    char        m_recvBuffer[BUF_MAX_SIZE];
    WSABUF      m_wsaRecvieBuf;
    int         m_iReadPos = 0;
    int         m_iWritePos = 0;
};

