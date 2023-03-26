#include "pch.h"
#include "AttackFireBall.h"
#include  "Collision.h"
AttackFireBall::AttackFireBall(s_InteractionObejct _owner, const XMFLOAT3& _vStartPos, const XMFLOAT3& _vRotateY) : m_Owner(_owner), m_fLifeTime(0)
{
	// Ÿ�Լ���
	m_eType = eInteractionType::AttackFireBall;

	// ��ġ�� ��ȯ�� ��ġ���� 2�������� ������ ����.
	XMFLOAT3 vecPos;
	XMVECTOR vecDirXM = XMLoadFloat3(&_vRotateY);
	XMVECTOR vecPosXM = XMLoadFloat3(&_vStartPos);
	vecDirXM = XMVectorScale(vecDirXM, 2.0f);
	
	XMStoreFloat3(&vecPos, vecDirXM + vecPosXM);
	m_transform->SetPos(vecPos);
	m_transform->SetYRotate(_vRotateY);
}

AttackFireBall::~AttackFireBall()
{

}


void AttackFireBall::Init()
{
	InteractionObject::Init();

	m_collision->SetCollisonClear(false);
}

void AttackFireBall::Update()
{
	m_fLifeTime += InGameUpdateManager::GetInstance()->GetDeltaTime();

	if (m_fLifeTime > m_fLifeMaxTime)
	{
		m_bVaildLife = false;
	}
		

	m_transform->SetMoveSpeed(5);

	m_transform->RotateMove();

	m_transform->Update();
}

void AttackFireBall::SendCollision()
{
	m_bVaildLife = false;
}
