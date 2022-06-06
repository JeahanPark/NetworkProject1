#pragma once

class Session;
class SessionManager
{

public:
	Session* CreateSession();

public:
	vector<Session*> GetSessions()
	{
		return m_vecSession;
	}

private:
	vector<Session*> m_vecSession;
};

