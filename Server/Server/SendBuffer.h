#pragma once
class SendBuffer
{
public:
    SendBuffer();
    ~SendBuffer();
public:
    WSABUF* GetWSABuf();
    char& GetSendBufferAdress();
private:
    char        m_sendBuffer[BUFSIZE];
    WSABUF     m_wsaSendBuf;
};
