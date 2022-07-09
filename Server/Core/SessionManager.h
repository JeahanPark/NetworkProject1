#pragma once

class Session;
class SessionManager
{

public:
	template <class T1>
	T1* CreateSession()
	{
		Session* session = new T1();
		m_vecSession.push_back(session);
		return (T1*)session;
	}

public:
	vector<Session*> GetSessions()
	{
		return m_vecSession;
	}

private:
	vector<Session*> m_vecSession;
};

