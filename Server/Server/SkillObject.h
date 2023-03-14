#pragma once
class SkillObject
{
public:
	SkillObject(eSkillType _skillType);
	~SkillObject();

public:
	void InitSkill();
	void Update();
	bool CanUseSkill();
	eSkillType GetType();

private:
	eSkillType m_eSkillType;
	float m_fCrtDelay;
	float m_fMaxDelay;
};

