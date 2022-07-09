#pragma once

class ReceiveBuffer;
class SendBuffer;
struct PacketData;
class Session
{
public:
    Session();
    ~Session();

public:
    SOCKET GetSocket() { return m_Socket; }
    void InitSession(HANDLE _iocpHandle, SOCKET _socket);

public:
    string GetSessionNumber();

    // ��û �Ҷ� ���
    void RegisterReceive();
    void RegisterSend(SendBuffer* _sendBuffer);

    // ���� �޾����� ���
    void ProcessReceive(DWORD _bytesTransferred);
    void ProcessSend(DWORD _bytesTransferred);

protected:
    virtual void PacketeHandle(PacketData* _Packetdata) = 0;

private:
    void Send();

private:
    //Receive
    ReceiveBuffer*          m_ReceiveBuffer = nullptr;

    // Send
    mutex                   m_lockSending;
    atomic<bool>            m_RegisterSendBufferAdd;

    list<SendBuffer*>     m_lisRegisterSendBuffer;
    list<SendBuffer*>     m_lisProcessSendBuffer;

private:
    SOCKET m_Socket = INVALID_SOCKET;
    int m_iSessionNumber = 0;
};

