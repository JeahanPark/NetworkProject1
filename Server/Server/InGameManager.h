#pragma once
class InGameObject;

class InGameManager : public BaseManager<InGameManager>
{
private:
	const int m_iMaxInGameUser = 10;

public:
	~InGameManager();

	s_InGameObject		CreateInGameObject(s_ServerSession _session);
	bool				InsertInGameObject(s_InGameObject _inGameObject);
	bool				DeleteInGameObject(s_ServerSession _session);
	bool				ExistInGameObject(int _iUserIndex);
public:
	s_InGameObject GetInGameObject(int _iUserIndex);
	const mapInGame GetmapInGame();

	// �̸��� ��������ߵ��� �𸣰��� �̳���� Interaction�� ����ִ¾ֵ鸸 �ִ°���
	void GetlistInGame(list<s_InGameObject>& _lisInGame);

private:
	// userIndex
	mapInGame			m_mapInGame;
	mutex				m_lockInGame;
};

