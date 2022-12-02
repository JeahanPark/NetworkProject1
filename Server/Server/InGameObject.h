#pragma once
class InGameObject
{
public:
	InGameObject(s_ServerSession _session);
	~InGameObject();

	bool SameSession(int _iUserIndex);
private:
	s_ServerSession m_session = nullptr;
};

