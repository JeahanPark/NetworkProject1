#include "pch.h"
#include "ClientSessionManager.h"

void ClientSessionManager::DeleteSession(s_ClientSession _session)
{
	LockGuard lock(m_lockSession);
	m_setSession.erase(_session);
}

set<s_ClientSession> ClientSessionManager::GetSessions()
{
	return m_setSession;
}