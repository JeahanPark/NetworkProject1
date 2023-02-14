#include "pch.h"
#include "AttackDummy.h"

AttackDummy::AttackDummy(XMFLOAT3 _vStartPos)
{
	m_eType = eInteractionType::AttackDummy;
	m_transform->SetPos(_vStartPos);
}

AttackDummy::~AttackDummy()
{
}

void AttackDummy::Update()
{
	// transform������Ʈ
	m_transform->Update();
}
