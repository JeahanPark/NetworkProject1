#include "pch.h"
#include "Collision.h"
#include "InteractionObject.h"
#include "Transform.h"
Collision::Collision(s_InteractionObejct _owner)
	: m_Owner(_owner),
	m_fClearCrtTime(0),
	m_fCollisionClearTime(0),
	m_fCollisionSize(1)

{
	switch (m_Owner->GetInteractionType())
	{
	case eInteractionType::User:
		m_CollisionType = (int)eCollisionType::Recive;
		break;
	case eInteractionType::AttackDummy:
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

			targetCollision->RecivedDamage();
		}

	}
}

bool Collision::AlreadDamaged(int _iInteractionIndex)
{
	for (auto iter : m_lisCollision)
	{
		if (iter == _iInteractionIndex)
			return true;
	}
	return false;
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

bool Collision::HaveCollisionType(eCollisionType _type)
{
	return (m_CollisionType & (int)_type) != 0;
}
