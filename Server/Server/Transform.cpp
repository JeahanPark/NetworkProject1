#include "pch.h"
#include "Transform.h"

Transform::Transform() :
	m_vPos{0,0,0}, m_fScale{0}, m_vYAxisDir{0,0,0}, m_fMoveSpeed{0}
{
}

Transform::~Transform()
{
}

void Transform::SetUserMove(const XMFLOAT3& _Dir, float _fSpeed)
{
	XMVECTOR vDir = XMLoadFloat3(&_Dir);
	XMVECTOR vMyDir = XMLoadFloat3(&m_vYAxisDir);
	vMyDir += vDir;
	XMVector3Normalize(vMyDir);

	XMStoreFloat3(&m_vYAxisDir, vMyDir);

	m_fMoveSpeed += _fSpeed;
	m_fMoveSpeed = m_fMoveSpeed > 3 ? 3 : m_fMoveSpeed;
}

void Transform::Update()
{
	// 이동할 속도가 있다.
	if (m_fMoveSpeed > 0)
	{
		XMVECTOR vMyDir = XMLoadFloat3(&m_vYAxisDir);
		XMVECTOR vPos = XMLoadFloat3(&m_vPos);
		vPos += vMyDir * (m_fMoveSpeed * InGameUpdateManager::GetInstance()->GetDeltaTime());

		XMStoreFloat3(&m_vPos, vPos);

		// 속도가 점점 떨어진다.
		m_fMoveSpeed -= 1 * InGameUpdateManager::GetInstance()->GetDeltaTime();
	}
}
