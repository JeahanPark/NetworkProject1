#include "pch.h"
#include "SessionManager.h"

Session* SessionManager::CreateSession()
{
	Session* session = new Session();
	m_vecSession.push_back(session);
	return session;
}
