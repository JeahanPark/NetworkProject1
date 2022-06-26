#include "pch.h"
#include "SocketEvent.h"

SocketEvent::SocketEvent()
{
}

SocketEvent::SocketEvent(SocketEventType type, Session* session) :
	m_EventType(type), m_Session(session)
{
}

void SocketEvent::SocketEventHandling(DWORD _bytesTransferred)
{
	switch (m_EventType)
	{
	case SocketEventType::SocketEventType_Send:
		m_Session->ProcessSend(_bytesTransferred);
		break;
	case SocketEventType::SocketEventType_Receive:
		m_Session->ProcessReceive(_bytesTransferred);
		break;
	default:
		break;
	}
}
