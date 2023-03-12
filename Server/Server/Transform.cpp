#include "pch.h"
#include "Transform.h"

Transform::Transform() :
	m_vPos{ 0,0,0 }, m_fScale{ 2 }, m_vMoveDir{ 0,0,0 }, m_fMoveSpeed{ 0 }, m_fMoveSpeedIncrease{ 3 }, m_vRotateY{0,0,-1}
{
}

Transform::~Transform()
{
}

void Transform::SetUserMove(const XMFLOAT3& _MoveDir, const XMFLOAT3& _RotateY, float _fSpeed)
{
	XMVECTOR vDir = XMLoadFloat3(&_MoveDir);
	XMVECTOR vMyDir = XMLoadFloat3(&m_vMoveDir);
	vMyDir += vDir;
	vMyDir = XMVector3Normalize(vMyDir);

	XMStoreFloat3(&m_vMoveDir, vMyDir);

	m_vRotateY = _RotateY;

	IncreaseSpeed(_fSpeed);
}

void Transform::SetAxisPos(eAxisType _eAxis, float _fValue)
{
	switch (_eAxis)
	{
	case eAxisType::Axis_X:
		m_vPos.x = _fValue;
		break;
	case eAxisType::Axis_Y:
		m_vPos.y = _fValue;
		break;
	case eAxisType::Axis_Z:
		m_vPos.z = _fValue;
		break;
	}
}

void Transform::SetYRotate(const XMFLOAT3& _vRotateY)
{
	m_vRotateY = _vRotateY;
}

void Transform::SetMoveSpeed(float _fSpeed)
{
	m_fMoveSpeed = _fSpeed;
}

void Transform::Update()
{
	// 이동할 속도가 있다.
	if (m_fMoveSpeed > 0)
	{
		XMVECTOR vMyDir = XMLoadFloat3(&m_vMoveDir);
		XMVECTOR vPos = XMLoadFloat3(&m_vPos);

		XMVECTOR speedMove = vMyDir * (m_fMoveSpeed * InGameUpdateManager::GetInstance()->GetDeltaTime());

		vPos += speedMove;

		XMStoreFloat3(&m_vPos, vPos);

		// 속도가 점점 떨어진다.
		m_fMoveSpeed -= 1 * InGameUpdateManager::GetInstance()->GetDeltaTime();
	}
}

void Transform::MovePos(const XMFLOAT3& _vDir)
{
	m_vMoveDir = _vDir;

	IncreaseSpeed(m_fMoveSpeedIncrease * InGameUpdateManager::GetInstance()->GetDeltaTime());
}

void Transform::RotateMove()
{
	m_vMoveDir = m_vRotateY;

	IncreaseSpeed(m_fMoveSpeedIncrease * InGameUpdateManager::GetInstance()->GetDeltaTime());
}

void Transform::IncreaseSpeed(float _fValue)
{
	m_fMoveSpeed += _fValue;
	m_fMoveSpeed = m_fMoveSpeed > 3 ? 3 : m_fMoveSpeed;
}
