#include "pch.h"
#include "ReflectionItem.h"

ReflectionItem::ReflectionItem(const XMFLOAT3& _vStartPos) : m_fLifeTime(0.f)
{
	// Ÿ�Լ���
	m_eType = eInteractionType::ReflectionItem;

	m_transform->SetPos(_vStartPos);
}

ReflectionItem::~ReflectionItem()
{
}

void ReflectionItem::Update()
{
	m_fLifeTime += InGameUpdateManager::GetInstance()->GetDeltaTime();

	if (m_fLifeTime >= m_fLifeMaxTime)
	{
		m_bVaildLife = false;
	}

	// transform������Ʈ
	m_transform->Update();
}

void ReflectionItem::Die()
{
	ContentsManager::GetInstance()->UpdateReflectionCrtCount(-1);
}
