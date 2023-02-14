#pragma once

class Transform
{
public:
	Transform();
	~Transform();

public:
	void SetPos(const XMFLOAT3& _vPos) { m_vPos = _vPos; }
	void SetUserMove(const XMFLOAT3& _Dir, float _fMoveSpeed);
	void Update();
public:
	const XMFLOAT3& GetPos() { return m_vPos; }
	const XMFLOAT3& GetYAxisDir() { return m_vYAxisDir; }
	float GetMoveSpeed() { return m_fMoveSpeed; }
private:
	float m_fMoveSpeed;

	// 위치
	XMFLOAT3 m_vPos;

	// 크기
	float m_fScale;

	// 방향
	XMFLOAT3 m_vYAxisDir;
};