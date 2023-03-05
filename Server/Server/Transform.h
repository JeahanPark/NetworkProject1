#pragma once

class Transform
{
public:
	Transform();
	~Transform();

public:
	void SetPos(const XMFLOAT3& _vPos) { m_vPos = _vPos; }
	void SetUserMove(const XMFLOAT3& _MoveDir, const XMFLOAT3& _RotateDir, float _fSpeed);
	void SetAxisPos(eAxisType _eAxis, float _fValue);
	void Update();
	void MovePos(const XMFLOAT3& _vDir);
public:
	const XMFLOAT3& GetPos() { return m_vPos; }
	const XMFLOAT3& GetMoveDir() { return m_vMoveDir; }
	const XMFLOAT3& GetRotateY() { return m_vRotateY; }
	float GetMoveSpeed() { return m_fMoveSpeed; }

private:
	void IncreaseSpeed(float _fValue);
private:
	float m_fMoveSpeed;
	float m_fMoveSpeedIncrease;
	// 위치
	XMFLOAT3 m_vPos;

	// 크기
	float m_fScale;

	// 이동방향
	XMFLOAT3 m_vMoveDir;

	// 회전
	XMFLOAT3 m_vRotateY;
};