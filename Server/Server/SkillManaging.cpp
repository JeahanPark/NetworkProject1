#include "pch.h"
#include "SkillManaging.h"
#include "SkillObject.h"
#include "UserObject.h"

SkillManaging::SkillManaging()
{
}

SkillManaging::~SkillManaging()
{
}

void SkillManaging::InitData(s_InteractionObejct _owner)
{
	m_owner = _owner;
}

void SkillManaging::InitSkill()
{

	m_lisSkill.clear();

	m_lisSkill.push_back(SkillObject(eSkillType::FireBall));
	//m_vecSkill.push_back(SkillObject(eSkillType::Wall));

	m_iterCrtSkill = m_lisSkill.begin();

	m_iterCrtSkill->InitSkill();
}

void SkillManaging::Update()
{
	if (!m_iterCrtSkill->Update())
	{
		NextSkill();
	}
}

bool SkillManaging::UseSkill()
{
	if (!m_iterCrtSkill->CanUseSkill())
		return false;

	// 원래같으면 skillObject상속받아서 타입별로 클래스 만들고 할텐데
	// 서버 코드니까 그냥 switch로 나눈다. 클라엿으면 나눳음
	switch (m_iterCrtSkill->GetType())
	{
	case eSkillType::FireBall:
		FireBall();
		break;
	case eSkillType::Wall:
		Wall();
		break;
	case eSkillType::Reflection:
		Reflection();
		break;
	default:
		break;
	}
	
	m_iterCrtSkill->UseSkill();


	return true;
}

bool SkillManaging::UsingSkill(eSkillType _type)
{
	if (m_iterCrtSkill->GetType() != _type)
		return false;

	return m_iterCrtSkill->GetUsingSkill();
}

void SkillManaging::AddSkill(eSkillType _skill)
{
	m_lisSkill.push_back(SkillObject(_skill));
}

eSkillType SkillManaging::GetCrtSkill()
{
	return m_iterCrtSkill->GetType();
}

void SkillManaging::FireBall()
{
	s_InteractionObejct interaction = InteractionCreator::CreateFireball(m_owner, m_owner->GetTransform()->GetPos(), m_owner->GetTransform()->GetRotateY());

	InteractionManager::GetInstance()->AddInteractionObject(interaction);
}

void SkillManaging::Wall()
{
}

void SkillManaging::Reflection()
{

}

void SkillManaging::NextSkill()
{
	if (m_iterCrtSkill->GetType() == eSkillType::Reflection)
	{
		list<SkillObject>::iterator removeIter = m_iterCrtSkill;
		m_iterCrtSkill++;
		m_lisSkill.erase(removeIter);
	}
	else
		m_iterCrtSkill++;

	if (m_iterCrtSkill == m_lisSkill.end())
		m_iterCrtSkill = m_lisSkill.begin();

	m_iterCrtSkill->InitSkill();

	PacketHandler::UpdateMySkill(m_owner);
}
