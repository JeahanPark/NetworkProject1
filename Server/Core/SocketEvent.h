#pragma once

enum class SocketEventType
{
	SocketEventType_None,
	SocketEventType_Send,
	SocketEventType_Receive,
	SocketEventType_Disconnect,
	SocketEventType_End
};

class Session;
class SendBuffer;
class SocketEvent : WSAOVERLAPPED
{
public:
	SocketEvent();
	SocketEvent(SocketEventType type, shared_session session);
	~SocketEvent();

public:
	shared_session GetSession()
	{
		return m_Session;
	}
	SocketEventType GetSocketEventType()
	{
		return m_EventType;
	}
	void SocketEventHandling(DWORD _bytesTransferred);

private:
	SocketEventType m_EventType = SocketEventType::SocketEventType_None;
	shared_session m_Session = nullptr;
};