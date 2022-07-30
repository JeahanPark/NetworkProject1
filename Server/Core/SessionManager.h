#pragma once

class Session;
class SessionManager
{

public:
	template <class T1>
	shared_session CreateSession()
	{
		LockGuard lock(m_lockSession);

		shared_session shared = make_shared<T1>();

		m_setSession.insert(shared);
		return shared;
	}

	void DeleteSession(shared_session _session);

	set<shared_session> GetSessions();


private:
	set<shared_session>		m_setSession;
	mutex					m_lockSession;
};

