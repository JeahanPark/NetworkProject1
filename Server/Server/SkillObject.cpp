#include "pch.h"
#include "SkillObject.h"

SkillObject::SkillObject(eSkillType _skillType) : m_eSkillType(_skillType), m_fCrtDelay(0.f), m_fMaxDelay(0.f), m_fCrtDuration(0.f), m_fMaxDuration(0.f), m_bUseSkill(false)
{
	switch (m_eSkillType)
	{
	case eSkillType::FireBall:
		m_fMaxDelay = 1;
		break;
	case eSkillType::Wall:
		m_fMaxDelay = 3;
		break;
	case eSkillType::Reflection:
		m_fMaxDelay = 3;
		m_fMaxDuration = 3;
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
	m_fMaxDelay = 0;
	m_bUseSkill = false;
}

bool SkillObject::Update()
{
	// 스킬 쿨타임 시간
	m_fCrtDelay += InGameUpdateManager::GetInstance()->GetDeltaTime();

	if (m_bUseSkill)
	{
		// 스킬 사용후 스킬유지시간
		m_fCrtDuration += InGameUpdateManager::GetInstance()->GetDeltaTime();

		if (m_fCrtDuration > m_fMaxDuration)
		{
			return false;
		}
	}

	return true;
}

bool SkillObject::CanUseSkill()
{
	return m_fCrtDelay > m_fMaxDelay;
}

eSkillType SkillObject::GetType()
{
	return m_eSkillType;
}

bool SkillObject::GetUsingSkill()
{
	return m_bUseSkill;
}

void SkillObject::UseSkill()
{
	m_bUseSkill = true;
}
