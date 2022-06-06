#pragma once

enum SocketEventType
{
	SocketEventType_None,
	SocketEventType_Send,
	SocketEventType_Receive,
	SocketEventType_End
};

class Session;
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

private:
	SocketEventType m_EventType = SocketEventType_None;
	Session* m_Session = nullptr;
};

