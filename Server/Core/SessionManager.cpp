#include "pch.h"
#include "SessionManager.h"

void SessionManager::DeleteSession(shared_session _session)
{
    LockGuard lock(m_lockSession);
    m_setSession.erase(_session);
}

set<shared_session> SessionManager::GetSessions()
{
    return m_setSession;
}
