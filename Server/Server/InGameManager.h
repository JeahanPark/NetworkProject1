#pragma once
class InGameObject;
class InGameManager : public BaseManager<ChattingManager>
{
private:
	const int m_iMaxInGameUser = 10;

public:
	~InGameManager();
	bool InsertChattingObject(s_ServerSession _session);
	bool DeleteChattingObject(s_ServerSession _session);
private:
	list<InGameObject*>	m_lisInGame;
	mutex				m_lockInGame;
};

