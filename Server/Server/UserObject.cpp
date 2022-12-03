#include "pch.h"
#include "UserObject.h"

UserObject::UserObject(s_UserController _uerController) 
	: m_userController(_uerController)
{
	m_eType = eInteractionType::User;
}

UserObject::~UserObject()
{
	m_userController = nullptr;
}
