#pragma once
class ClientSessionManager : public BaseManager<ClientSessionManager>
{
public:
	s_ClientSession CreateSession()
	{
		LockGuard lock(m_lockSession);

		s_ClientSession shared = make_shared<ClientSession>();

		m_setSession.insert(shared);
		return shared;
	}

	void DeleteSession(s_ClientSession _session);

	set<s_ClientSession> GetSessions();


private:
	set<s_ClientSession>		m_setSession;
	mutex						m_lockSession;
};

