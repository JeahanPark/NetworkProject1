#pragma once
class InGameObject
{
public:
	InGameObject(s_ServerSession _session, s_UserController _userController);
	~InGameObject();

public:
	bool SameSession(int _iUserIndex);
	void MyUserMove(MyUserMovePacket* _packetData);

public:
	const s_ServerSession GetSession() { return m_session; }
	s_UserController GetUserController() { return m_UserController; }
	s_InteractionObejct	GetUserInteraction() { return m_userInteraction; }
public:
	void SetUser(s_InteractionObejct _user);

private:
	s_InteractionObejct m_userInteraction;
	s_ServerSession		m_session = nullptr;
	s_UserController	m_UserController = nullptr;
};

