#include "pch.h"
#include "SocketEvent.h"

SocketEvent::SocketEvent()
{
}

SocketEvent::SocketEvent(SocketEventType type, shared_session session) :
	m_EventType(type), m_Session(session)
{
}

SocketEvent::~SocketEvent()
{
	m_Session = nullptr;
}

void SocketEvent::SocketEventHandling(DWORD _bytesTransferred)
{
	switch (m_EventType)
	{
	case SocketEventType::SocketEventType_Send:
		m_Session->ProcessSend(_bytesTransferred);
		break;
	case SocketEventType::SocketEventType_Receive:
		cout << m_Session->GetSessionNumber() << "SocketEventType_Receive" << endl;
		m_Session->ProcessReceive(_bytesTransferred);
		break;
	case SocketEventType::SocketEventType_Disconnect:
		m_Session->ProcessDisconnect();
		break;
	default:
		break;
	}
}
