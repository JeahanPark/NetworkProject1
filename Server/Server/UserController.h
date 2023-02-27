#pragma once

class UserController
{
public:
	UserController();
	~UserController();
public:
	void SetUserMove(MyUserMovePacket* _packetData);
	void SetVaildCheck(bool _bVaild);

public:
	bool GetNeedMoveCheck() { return m_bNeedMoveCheck; }
	const XMFLOAT3& GetMoveDir() { return m_vMoveDir; }
	float GetMoveSpeed() { return m_fMoveSpeed; }
	bool GetVaildCheck() { return m_bValidCheck; }
public:
	void ClearUserCommand();

private:
	bool m_bValidCheck;
	bool m_bNeedMoveCheck;
	XMFLOAT3 m_vMoveDir;
	float m_fMoveSpeed;
};

