#pragma once
class InGameObject
{
public:
	InGameObject(s_ServerSession _session, s_UserController _userController);
	~InGameObject();

public:
	bool SameSession(int _iUserIndex);
	void MyUserMove(MyUserMovePacket* _packetData);
private:
	s_ServerSession		m_session = nullptr;
	s_UserController	m_UserController = nullptr;
};

