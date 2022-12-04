#pragma once

class UserController
{
public:
	UserController();
	~UserController();
public:
	void SetUserMove(MyUserMovePacket* _packetData);

public:
	bool GetNeedMoveCheck() { return m_bNeedMoveCheck; }
	const XMFLOAT3& GetMoveDir() { return m_vMoveDir; }

public:
	void ClearUserCommand();

private:
	bool m_bNeedMoveCheck;
	XMFLOAT3 m_vMoveDir;
};

