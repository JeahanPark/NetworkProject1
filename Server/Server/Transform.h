#pragma once

class Transform
{
public:
	Transform();
	~Transform();

public:
	void SetUserMove(const XMFLOAT3& _Dir, float _fMoveSpeed);
	void Update();
public:
	const XMFLOAT3& GetPos() { return m_vPos; }
private:
	float m_fMoveSpeed;

	// 위치
	XMFLOAT3 m_vPos;

	// 크기
	float m_fScale;

	// 방향
	XMFLOAT3 m_vYAxisDir;
};