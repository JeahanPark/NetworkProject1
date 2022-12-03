#include "pch.h"
#include "InteractionManager.h"
#include "InteractionObject.h"
#include "UserObject.h"
InteractionManager::~InteractionManager()
{
	for (InteractionObject* iter : m_lisInteraction)
		delete iter;
}

void InteractionManager::AddUserInteractionObject(
	s_InGameObject _ingameOBject,
	s_UserController _userController)
{
	UserObject* user = new UserObject(_userController);

	m_lisInteraction.push_back(user);
}
