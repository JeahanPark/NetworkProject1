#pragma once
#include "InteractionObject.h"

class State;
class UserObject : public InteractionObject
{
public:
	UserObject(s_UserController _uerController, const UserData* _userData);
	~UserObject();

public:
	// InteractionObject을(를) 통해 상속됨
	virtual void Update() override;

	void SettingInitialInGameDataPacket(InitialInGameData* _packet);

private:
	s_UserController		m_userController = nullptr;
	State*					m_state = nullptr;
	UserData				m_UserData;
};

