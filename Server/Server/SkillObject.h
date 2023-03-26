#pragma once
class SkillObject
{
public:
	SkillObject(eSkillType _skillType);
	~SkillObject();

public:
	void InitSkill();
	bool Update();
	bool CanUseSkill();
	eSkillType GetType();
	
	void UseSkill();

private:
	eSkillType m_eSkillType;
	float m_fCrtDelay;
	float m_fMaxDelay;

	bool m_bUseSkill;
	float m_fCrtDuration;
	float m_fMaxDuration;
};

