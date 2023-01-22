#pragma once
class SendBuffer
{
public:
    SendBuffer(int _BufferSize);
    ~SendBuffer();
public:
    WSABUF* GetWSABuf();
    char* GetSendBufferAdress(int _startBuffer = 0);
    void WsaBufSetting();
private:
    int         m_BufferSize;
    char*       m_sendBuffer;
    WSABUF      m_wsaSendBuf;
};
