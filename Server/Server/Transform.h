#pragma once

class Transform
{
public:
	Transform();
	~Transform();

public:
	void MovePos(const XMFLOAT3& _Dir, float _fSpeed);
	void Update();
public:
	const XMFLOAT3& GetPos() { return m_vPos; }
private:
	// 위치
	XMFLOAT3 m_vPos;

	// 크기
	float m_fScale;

	// 방향
	XMFLOAT3 m_vYAxisDir;
};