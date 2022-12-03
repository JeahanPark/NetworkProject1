#pragma once
class InGameObject;
typedef std::map<int, s_InGameObject> InGameMap;
class InGameManager : public BaseManager<InGameManager>
{
private:
	const int m_iMaxInGameUser = 10;

public:
	~InGameManager();
	bool InsertInGameObject(s_ServerSession _session);
	bool DeleteInGameObject(s_ServerSession _session);
	s_InGameObject GetInGameObject(int _iUserIndex);
	 
private:
	void CreateInGameObject(s_ServerSession _session);

private:
	// userIndex
	InGameMap			m_mapInGame;
	mutex				m_lockInGame;
};

