#pragma once

class SkillObject;
class SkillManaging
{
public:
	SkillManaging(InteractionObject* _owner);
	~SkillManaging();

public:
	void InitSkill();
	void Update();
	bool CanUseSkill();
	void UseSkill();
	eSkillType GetCrtSkill();

private:
	SkillObject& CrtUseSkill();
	void FireBall();
	void Wall();

private:
	InteractionObject* m_owner;

	int m_iCrtUseSkillIndex;

	vector<SkillObject> m_vecSkill;
};

