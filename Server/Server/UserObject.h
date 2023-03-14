#pragma once
#include "InteractionObject.h"
#include "SkillManaging.h"

class UserObject : public InteractionObject
{
public:
	UserObject(s_UserController _uerController, const UserData* _userData);
	~UserObject();

public:
	// InteractionObject을(를) 통해 상속됨
	virtual void Update() override;
	void SettingInitialInGameDataPacket(InitialInGameData* _packet);
	int GetUserIndex();
	void UseSkiil();
	eSkillType GetCrtSkill();

private:
	s_UserController		m_userController = nullptr;
	UserData				m_UserData;
	SkillManaging			m_SkillManaging;
};

