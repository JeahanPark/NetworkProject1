#include "pch.h"
#include "Collision.h"
#include "InteractionObject.h"
#include "Transform.h"
Collision::Collision(s_InteractionObejct _owner)
	: m_Owner(_owner),
	m_fClearCrtTime(0),
	m_fCollisionClearTime(0),
	m_fCollisionSize(4)

{
}

Collision::~Collision()
{
}


void Collision::Update(const list<s_InteractionObejct>& _lisInteractin)
{
	for (auto iter : _lisInteractin)
	{
		Collision* targetCollision = iter->GetCollision();

		int targetIndex = targetCollision->GetInteractionIndex();
		if (targetIndex == m_Owner->GetInteractionIndex())
		{
			// 내꺼다.
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

		vMyPos 
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
}

int Collision::GetInteractionIndex()
{
	return m_Owner->GetInteractionIndex();
}
