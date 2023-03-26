#pragma once

class SkillObject;
class SkillManaging
{
public:
	SkillManaging();
	~SkillManaging();

public:
	void InitData(s_InteractionObejct _owner);

	void InitSkill();
	void Update();
	bool UseSkill();
	void AddSkill(eSkillType _skill);
	eSkillType GetCrtSkill();

private:
	void FireBall();
	void Wall();
	void Reflection();
	void NextSkill();

private:
	s_InteractionObejct m_owner;

	list<SkillObject>::iterator m_iterCrtSkill;

	list<SkillObject> m_lisSkill;
};

