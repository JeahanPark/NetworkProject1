#pragma once
#include "InteractionObject.h"


class UserObject : public InteractionObject
{
public:
	UserObject(s_UserController _uerController, const UserData* _userData);
	~UserObject();

public:
	// InteractionObject��(��) ���� ��ӵ�
	virtual void Update() override;

	void SettingInitialInGameDataPacket(InitialInGameData* _packet);
	int GetUserIndex();
private:
	s_UserController		m_userController = nullptr;
	UserData				m_UserData;
};

