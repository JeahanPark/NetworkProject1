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

	// ���������� skillObject��ӹ޾Ƽ� Ÿ�Ժ��� Ŭ���� ����� ���ٵ�
	// ���� �ڵ�ϱ� �׳� switch�� ������. Ŭ������ ������
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
