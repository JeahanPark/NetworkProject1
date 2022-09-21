#pragma once
class ServerSessionManager : public BaseManager<ServerSessionManager>
{
public:
	s_ServerSession CreateSession()
	{
		LockGuard lock(m_lockSession);

		s_ServerSession shared = make_shared<ServerSession>();

		m_setSession.insert(shared);
		return shared;
	}

	void DeleteSession(s_ServerSession _session);

	set<s_ServerSession> GetSessions();


private:
	set<s_ServerSession>		m_setSession;
	mutex						m_lockSession;
};

