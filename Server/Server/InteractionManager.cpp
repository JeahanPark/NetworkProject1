#include "pch.h"
#include "InteractionManager.h"
#include "InteractionObject.h"
#include "UserObject.h"
InteractionManager::~InteractionManager()
{
	for (InteractionObject* iter : m_lisInteraction)
		delete iter;
}

void InteractionManager::AddUserInteractionObject(int _userIndex, s_UserController _userController)
{
	LockGuard lock(m_lockInteraction);

	UserObject* user = new UserObject(_userController, _userIndex);

	m_lisInteraction.push_back(user);
}

void InteractionManager::AllUpdateInteractionObject()
{
	LockGuard lock(m_lockInteraction);

	for (InteractionObject* iter : m_lisInteraction)
		iter->Update();
}
