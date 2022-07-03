#pragma once
class SendBuffer
{
public:
    SendBuffer(int _BufferSize);
    ~SendBuffer();
public:
    WSABUF* GetWSABuf();
    char* GetSendBufferAdress();
    void WsaBufSetting();
private:
    int         m_BufferSize;
    char*       m_sendBuffer;
    WSABUF      m_wsaSendBuf;
};
