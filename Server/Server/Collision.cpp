#include "pch.h"
#include "Collision.h"
#include "InteractionObject.h"
#include "Transform.h"
Collision::Collision(s_InteractionObejct _owner)
	: m_Owner(_owner),
	m_fCollisionClearTime(1),
	m_fCollisionSize(1),
	m_bClear(true)

{
	switch (m_Owner->GetInteractionType())
	{
	case eInteractionType::User:
		m_CollisionType = (int)eCollisionType::Recive;
		break;
	case eInteractionType::AttackDummy:
		m_CollisionType = (int)eCollisionType::Send;
		break;
	case eInteractionType::AttackFireBall:
		m_CollisionType = (int)eCollisionType::Send;
		break;
	default:
		m_CollisionType = (int)eCollisionType::None;
	}
}

Collision::~Collision()
{
}


void Collision::Update(const list<s_InteractionObejct>& _lisInteractin)
{
	// 쿨타임 끝난 애들 없애기
	CleanUpCollisionList();

	// 내가 남한테 충돌하는 놈인지 체크
	if (!HaveCollisionType(eCollisionType::Send))
		return;

	for (auto iter : _lisInteractin)
	{
		Collision* targetCollision = iter->GetCollision();

		int targetIndex = targetCollision->GetInteractionIndex();
		if (targetIndex == m_Owner->GetInteractionIndex())
		{
			// 내꺼다.
			continue;
		}

		if (!targetCollision->HaveCollisionType(eCollisionType::Recive))
		{
			// 맞을수있는애가 아니다.
			continue;
		}

		if (AlreadDamaged(targetIndex))
		{
			// 이미 맞았다.
			continue;
		}

		// 충돌 검사
		Transform* targetTransform = targetCollision->GetTransform();
		
		XMVECTOR vTargetPos = XMLoadFloat3(&targetTransform->GetPos());
		XMVECTOR vMyPos = XMLoadFloat3(&m_Owner->GetTransform()->GetPos());

		XMVECTOR vSub = XMVectorSubtract(vMyPos, vTargetPos);
		XMVECTOR length = XMVector3Length(vSub);

		float fDistance = 0.0f;
		XMStoreFloat(&fDistance, length);

		if (fDistance < m_fCollisionSize)
		{
			// 충돌 했다.
			m_lisCollision.push_back({targetIndex, InGameUpdateManager::GetInstance()->GetTotalDeltaTime()});
			targetCollision->RecivedDamage();
		}

	}
}

bool Collision::AlreadDamaged(int _iInteractionIndex)
{
	for (auto iter : m_lisCollision)
	{
		// 같은 인덱스인지 확인
		if (iter.m_interactionIndex == _iInteractionIndex)
			return true;
	}
	return false;
}

void Collision::CleanUpCollisionList()
{
	if (!m_bClear)
		return;

	auto iter = m_lisCollision.begin();

	double totalDeltaTime = InGameUpdateManager::GetInstance()->GetTotalDeltaTime();
	// 인터렉션 Update할것들 하기
	while (iter != m_lisCollision.end())
	{
		// 시간체크를 해서 쿨타임이 지났으면 없앤다.
		if (totalDeltaTime - iter->m_dRecordTotalDeltaTime > m_fCollisionClearTime)
		{
			iter = m_lisCollision.erase(iter);
			continue;
		}
		else
			++iter;
	}
}

void Collision::RecivedDamage()
{
	m_Owner->RecivedDamage();
}

int Collision::GetInteractionIndex()
{
	return m_Owner->GetInteractionIndex();
}

Transform* Collision::GetTransform()
{
	return  m_Owner->GetTransform();
}

void Collision::SetCollisonClear(bool _bCleer)
{
	m_bClear = _bCleer;
}

bool Collision::HaveCollisionType(eCollisionType _type)
{
	return (m_CollisionType & (int)_type) != 0;
}
