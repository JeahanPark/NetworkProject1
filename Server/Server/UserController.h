#pragma once
class UserController
{
public:
	UserController();
	~UserController();
public:
	void SetUserMove(MyUserMovePacket* _packetData);
	void ClearUserCommand();

private:
	float m_fDirX;
	float m_fDirY;
};

