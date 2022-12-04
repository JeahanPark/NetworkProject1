#pragma once

class Transform
{
public:
	Transform();
	~Transform();

public:
	void MovePos(const XMFLOAT3& _Dir, float _fSpeed);

public:
	const XMFLOAT3& GetPos() { return m_vPos; }
private:
	// ��ġ
	XMFLOAT3 m_vPos;

	// ũ��
	float m_fScale;

	// ����
	XMFLOAT3 m_vYAxisDir;
};