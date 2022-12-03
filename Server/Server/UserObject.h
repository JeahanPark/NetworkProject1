#pragma once
#include "InteractionObject.h"


class UserObject : public InteractionObject
{
public:
	UserObject(s_UserController _uerController);
	~UserObject();

private:
	s_UserController m_userController = nullptr;
};

