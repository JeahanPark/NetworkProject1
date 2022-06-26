#pragma once

class ReceiveBuffer;
class SendBuffer;
class Session
{
public:
    Session();
    ~Session();

public:
    SOCKET GetSocket() { return m_Socket; }
    void SetSocket(SOCKET _socket) { m_Socket = _socket; }

public:
    string GetSessionNumber();

    // ��û �Ҷ� ���
    void RegisterReceive();
    void RegisterSend(SendBuffer* _sendBuffer);

    // ���� �޾����� ���
    void ProcessReceive(DWORD _bytesTransferred);
    void ProcessSend(DWORD _bytesTransferred);

private:
    //Receive
    ReceiveBuffer*          m_ReceiveBuffer = nullptr;

    // Send
    vector<SendBuffer*>     m_vecRegisterSendBuffer;
    vector<SendBuffer*>     m_vecProcessSendBuffer;

private:
    SOCKET m_Socket = INVALID_SOCKET;
    int m_iSessionNumber = 0;
};

