#pragma once

class ReceiveBuffer;
class SendBuffer;
struct BasePacket;

// enable_shared_from_this
// enable_shared_from_this���� �Ļ��� ��ü�� ��� �Լ����� shared_from_this
// �޼��带 ����Ͽ� ���� shared_ptr �����ڿ� �������� �����ϴ�
// �ν��Ͻ��� shared_ptr �����ڸ� ����ϴ�. 

class Session : public enable_shared_from_this<Session>
{
public:
    Session();
    ~Session();

public:
    SOCKET GetSocket() { return m_socket; }
    void InitSession(HANDLE _iocpHandle, SOCKET _socket);

public:
    string GetSessionNumber();
    bool IsSessionConnect();

    // ��û �Ҷ� ���
    void RegisterReceive();
    void RegisterSend(SendBuffer* _sendBuffer);
    void RegisterDisconnect();

    // ���� �޾����� ���
    void ProcessReceive(DWORD _bytesTransferred);
    void ProcessSend(DWORD _bytesTransferred);
    void ProcessDisconnect();

    void SocketEventError(int _iCode);

protected:
    virtual void PacketeHandle(BasePacket* _Packetdata) = 0;
    virtual void DeleteSession() = 0;
private:
    void Send();

private:
    //Receive
    ReceiveBuffer*          m_ReceiveBuffer = nullptr;

    // Send
    mutex                   m_lockSending;
    atomic<bool>            m_RegisterSendBufferAdd;

    list<SendBuffer*>       m_lisRegisterSendBuffer;
    list<SendBuffer*>       m_lisProcessSendBuffer;

    //disconnent
    atomic<bool>            m_RegisterDisconnect; // �̰Ŵ� disconnect�� ��û�ߴ�.
    bool                    m_bSessionDisconnect; // �̰Ŵ� disconnectó���� �Ϸ�ƴ�.

private:
    SOCKET m_socket = INVALID_SOCKET;
    int m_iSessionNumber = 0;
};

