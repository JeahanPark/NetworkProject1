#pragma once
class InGameObject;

class InGameManager : public BaseManager<InGameManager>
{
private:
	const int m_iMaxInGameUser = 10;

public:
	~InGameManager();
	bool InsertInGameObject(s_ServerSession _session);
	bool DeleteInGameObject(s_ServerSession _session);

public:
	s_InGameObject GetInGameObject(int _iUserIndex);
	const mapInGame GetmapInGame();

	// �̸��� ��������ߵ��� �𸣰��� �̳���� Interaction�� ����ִ¾ֵ鸸 �ִ°���
	void GetlistInGame(list<s_InGameObject>& _lisInGame);

private:
	void CreateInGameObject(s_ServerSession _session);

private:
	// userIndex
	mapInGame			m_mapInGame;
	mutex				m_lockInGame;
};

