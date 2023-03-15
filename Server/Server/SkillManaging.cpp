#include "pch.h"
#include "SkillManaging.h"
#include "SkillObject.h"
#include "UserObject.h"

SkillManaging::SkillManaging(InteractionObject* _owner) : m_iCrtUseSkillIndex(0), m_owner(_owner)
{
}

SkillManaging::~SkillManaging()
{
}

void SkillManaging::InitSkill()
{
	m_iCrtUseSkillIndex = 0;

	m_vecSkill.clear();

	m_vecSkill.push_back(SkillObject(eSkillType::FireBall));
	//m_vecSkill.push_back(SkillObject(eSkillType::Wall));

	SkillObject& skill = CrtUseSkill();
	skill.InitSkill();
}

void SkillManaging::Update()
{
	SkillObject& skill = CrtUseSkill();

	skill.Update();
}

bool SkillManaging::UseSkill()
{
	SkillObject& skill = CrtUseSkill();

	if (!skill.CanUseSkill())
		return false;

	// 원래같으면 skillObject상속받아서 타입별로 클래스 만들고 할텐데
	// 서버 코드니까 그냥 switch로 나눈다. 클라엿으면 나눳음
	switch (skill.GetType())
	{
	case eSkillType::FireBall:
		FireBall();
		break;
	case eSkillType::Wall:
		Wall();
		break;
	default:
		break;
	}

	++m_iCrtUseSkillIndex;

	if (m_iCrtUseSkillIndex >= m_vecSkill.size())
		m_iCrtUseSkillIndex = 0;

	skill = CrtUseSkill();
	skill.InitSkill();

	return true;
}

eSkillType SkillManaging::GetCrtSkill()
{
	return CrtUseSkill().GetType();
}

SkillObject& SkillManaging::CrtUseSkill()
{
	return m_vecSkill[m_iCrtUseSkillIndex];
}

void SkillManaging::FireBall()
{
	s_InteractionObejct interaction = InteractionCreator::CreateFireball(m_owner->GetTransform()->GetPos(), m_owner->GetTransform()->GetRotateY());

	InteractionManager::GetInstance()->AddInteractionObject(interaction);
}

void SkillManaging::Wall()
{
}
