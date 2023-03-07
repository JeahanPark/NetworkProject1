#include "pch.h"
#include "AttackFireBall.h"
#include  "Collision.h"
AttackFireBall::AttackFireBall(XMFLOAT3 _vStartPos, XMFLOAT3 _vMoveDir) : m_vMoveDir(_vMoveDir), m_fLifeTime(0)
{
	// 타입세팅
	m_eType = eInteractionType::AttackFireBall;

	// 위치를 소환된 위치에서 2방향정도 앞으로 간다.
	XMFLOAT3 vecPos;
	XMVECTOR vecDirXM = XMLoadFloat3(&_vMoveDir);
	XMVECTOR vecPosXM = XMLoadFloat3(&_vStartPos);
	vecDirXM = XMVectorScale(vecDirXM, 2.0f);
	
	XMStoreFloat3(&vecPos, vecDirXM + vecPosXM);
	m_transform->SetPos(vecPos);
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
		m_bVaildLife = false;

	m_transform->MovePos(m_vMoveDir);

	m_transform->Update();
}
