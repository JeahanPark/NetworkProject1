#include "pch.h"
#include "AttackDummy.h"

AttackDummy::AttackDummy(const XMFLOAT3& _vStartPos) : m_bMoveUp(false)
{
	m_eType = eInteractionType::AttackDummy;
	m_transform->SetPos(_vStartPos);
}

AttackDummy::~AttackDummy()
{
}

void AttackDummy::Update()
{
	if (m_bMoveUp)
	{
		// 위로 이동
		m_transform->MovePos(FowardVector);

		if (m_transform->GetPos().z > 5)
			m_bMoveUp = false;
	}
	else
	{
		// 아래 이동

		m_transform->MovePos(BackVector);

		if (m_transform->GetPos().z < -5)
			m_bMoveUp = true;
	}

	// transform업데이트
	m_transform->Update();
}
