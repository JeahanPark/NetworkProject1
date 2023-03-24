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
	eSkillType GetCrtSkill();

private:
	SkillObject& CrtUseSkill();
	void FireBall();
	void Wall();

private:
	s_InteractionObejct m_owner;

	int m_iCrtUseSkillIndex;

	vector<SkillObject> m_vecSkill;
};

