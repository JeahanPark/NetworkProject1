#pragma once

class ReceiveBuffer;
class SendBuffer;
struct PacketData;

// enable_shared_from_this
// enable_shared_from_this에서 파생된 개체는 멤버 함수에서 shared_from_this
// 메서드를 사용하여 기존 shared_ptr 소유자와 소유권을 공유하는
// 인스턴스의 shared_ptr 소유자를 만듭니다. 

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

    // 요청 할때 사용
    void RegisterReceive();
    void RegisterSend(SendBuffer* _sendBuffer);
    void RegisterDisconnect();

    // 응답 받았을때 사용
    void ProcessReceive(DWORD _bytesTransferred);
    void ProcessSend(DWORD _bytesTransferred);
    void ProcessDisconnect();

    void SocketEventError(int _iCode);

protected:
    virtual void PacketeHandle(PacketData* _Packetdata) = 0;

private:
    void Send();

private:
    atomic<int>             m_refCount;
    
    //Receive
    ReceiveBuffer*          m_ReceiveBuffer = nullptr;

    // Send
    mutex                   m_lockSending;
    atomic<bool>            m_RegisterSendBufferAdd;

    list<SendBuffer*>       m_lisRegisterSendBuffer;
    list<SendBuffer*>       m_lisProcessSendBuffer;

    //disconnent
    atomic<bool>            m_RegisterDisconnet;

private:
    SOCKET m_socket = INVALID_SOCKET;
    int m_iSessionNumber = 0;
};

