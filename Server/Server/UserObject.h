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
	virtual void	Update() override;
	virtual void	Init();
	bool			UseSkiil();
	virtual void	RecivedCollision(Collision* _sendtarget);
	void			AddPoint(long _lAddPoint);

public:
	int GetUserIndex();
	eSkillType GetCrtSkill();

	void SettingInitialInGameDataPacket(InitialInGameData* _packet);

private:
	void ReciveDamage(int _iDamage);

private:
	s_UserController		m_userController = nullptr;
	UserData				m_UserData;
	SkillManaging			m_SkillManaging;
};

