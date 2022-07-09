#pragma once

enum class SocketEventType
{
	SocketEventType_None,
	SocketEventType_Send,
	SocketEventType_Receive,
	SocketEventType_End
};

class Session;
class SendBuffer;
class SocketEvent : WSAOVERLAPPED
{
public:
	SocketEvent();
	SocketEvent(SocketEventType type, Session* session);

public:
	Session* GetSession()
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
	Session* m_Session = nullptr;
};