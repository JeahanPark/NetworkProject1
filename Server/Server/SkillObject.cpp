#include "pch.h"
#include "SkillObject.h"

SkillObject::SkillObject(eSkillType _skillType) : m_eSkillType(_skillType), m_fCrtDelay(0.f), m_fMaxDelay(0.f)
{
	switch (m_eSkillType)
	{
	case eSkillType::FireBall:
		m_fMaxDelay = 1;
		break;
	case eSkillType::Wall:
		m_fMaxDelay = 3;
		break;
	default:
		break;
	}
}

SkillObject::~SkillObject()
{
}

void SkillObject::InitSkill()
{
	m_fCrtDelay = 0;
}

void SkillObject::Update()
{
	m_fCrtDelay += InGameUpdateManager::GetInstance()->GetDeltaTime();
}

bool SkillObject::CanUseSkill()
{
	return m_fCrtDelay > m_fMaxDelay;
}

eSkillType SkillObject::GetType()
{
	return m_eSkillType;
}
