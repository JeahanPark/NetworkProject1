#include "pch.h"
#include "SocketEvent.h"

SocketEvent::SocketEvent()
{
}

SocketEvent::SocketEvent(SocketEventType type, Session* session) :
	m_EventType(type), m_Session(session)
{
}
