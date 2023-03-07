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
	// ��Ÿ�� ���� �ֵ� ���ֱ�
	CleanUpCollisionList();

	// ���� ������ �浹�ϴ� ������ üũ
	if (!HaveCollisionType(eCollisionType::Send))
		return;

	for (auto iter : _lisInteractin)
	{
		Collision* targetCollision = iter->GetCollision();

		int targetIndex = targetCollision->GetInteractionIndex();
		if (targetIndex == m_Owner->GetInteractionIndex())
		{
			// ������.
			continue;
		}

		if (!targetCollision->HaveCollisionType(eCollisionType::Recive))
		{
			// �������ִ¾ְ� �ƴϴ�.
			continue;
		}

		if (AlreadDamaged(targetIndex))
		{
			// �̹� �¾Ҵ�.
			continue;
		}

		// �浹 �˻�
		Transform* targetTransform = targetCollision->GetTransform();
		
		XMVECTOR vTargetPos = XMLoadFloat3(&targetTransform->GetPos());
		XMVECTOR vMyPos = XMLoadFloat3(&m_Owner->GetTransform()->GetPos());

		XMVECTOR vSub = XMVectorSubtract(vMyPos, vTargetPos);
		XMVECTOR length = XMVector3Length(vSub);

		float fDistance = 0.0f;
		XMStoreFloat(&fDistance, length);

		if (fDistance < m_fCollisionSize)
		{
			// �浹 �ߴ�.
			m_lisCollision.push_back({targetIndex, InGameUpdateManager::GetInstance()->GetTotalDeltaTime()});
			targetCollision->RecivedDamage();
		}

	}
}

bool Collision::AlreadDamaged(int _iInteractionIndex)
{
	for (auto iter : m_lisCollision)
	{
		// ���� �ε������� Ȯ��
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
	// ���ͷ��� Update�Ұ͵� �ϱ�
	while (iter != m_lisCollision.end())
	{
		// �ð�üũ�� �ؼ� ��Ÿ���� �������� ���ش�.
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
