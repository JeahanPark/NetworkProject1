#pragma once

class UserController
{
public:
	UserController();
	~UserController();
public:
	void SetUserMove(MyUserMovePacket* _packetData);
	void SetVaildCheck(bool _bVaild);
	void SetDie(bool _bDie);
public:
	bool GetNeedMoveCheck() { return m_bNeedMoveCheck; }
	const XMFLOAT3& GetMoveDir() { return m_vMoveDir; }
	float GetMoveSpeed() { return m_fMoveSpeed; }
	bool GetVaildCheck() { return m_bValidCheck; }
	bool GetDie() { return m_bDie; }
public:
	void ClearUserCommand();

private:
	bool m_bDie;
	bool m_bValidCheck;
	bool m_bNeedMoveCheck;
	XMFLOAT3 m_vMoveDir;
	float m_fMoveSpeed;
};

