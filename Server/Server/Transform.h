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
	const float& GetMoveSpeed() { return m_fMoveSpeed; }
	const XMFLOAT3& GetYAxisDir() { return m_vYAxisDir; }
private:
	float m_fMoveSpeed;

	// ��ġ
	XMFLOAT3 m_vPos;

	// ũ��
	float m_fScale;

	// ����
	XMFLOAT3 m_vYAxisDir;
};