#include "pch.h"
#include "InteractionManager.h"
#include "InteractionObject.h"
#include "UserObject.h"
InteractionManager::~InteractionManager()
{
	m_lisInteraction.clear();
}

void InteractionManager::AddUserInteractionObject(int _userIndex, s_UserController _userController)
{
	LockGuard lock(m_lockInteraction);

	UserObject* object = new UserObject(_userController, _userIndex);

	m_lisInteraction.push_back(object);
}

//일부로 &안붙임 &해서 꺼내서 쓰다가 추가되면 안돼서
list<InteractionObject*> InteractionManager::AllUpdateInteractionObject()
{
	LockGuard lock(m_lockInteraction);

	for (auto iter = m_lisInteraction.begin(); iter != m_lisInteraction.end(); ++iter)
	{
		InteractionObject* object = *iter;
		object->Update();

		// 삭제해야될 오브젝트일경우 생명주기가 끝나거나 등등
		if (!object->GetValidCheck())
		{
			auto removeIter = iter;
			++iter;
			m_lisInteraction.erase(removeIter);
			delete *removeIter;
			continue;
		}
	}

	return m_lisInteraction;
}