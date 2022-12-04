#include "pch.h"
#include "Transform.h"

Transform::Transform() :
	m_vPos{0,0,0}, m_fScale{}, m_vYAxisDir{0,0,0}
{
}

Transform::~Transform()
{
}

void Transform::MovePos(const XMFLOAT3& _Dir, float _fSpeed)
{
	m_vPos.x += _Dir.x * _fSpeed;
	m_vPos.z += _Dir.z * _fSpeed;
}
