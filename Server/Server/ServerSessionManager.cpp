#include "pch.h"
#include "ServerSessionManager.h"

void ServerSessionManager::DeleteSession(s_ServerSession _session)
{
	LockGuard lock(m_lockSession);
	m_setSession.erase(_session);
}

set<s_ServerSession> ServerSessionManager::GetSessions()
{
	return m_setSession;
}