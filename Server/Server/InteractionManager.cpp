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

void InteractionManager::AddDeleteInteraction(list<InteractionObject*>& _InteractionObjects)
{
	for (auto iter : m_lisDeleteInteraction)
	{
		_InteractionObjects.push_back(iter);
	}
}

void InteractionManager::ClearDeleteInteraction()
{
	for (auto iter : m_lisDeleteInteraction)
	{
		delete iter;
	}
	m_lisDeleteInteraction.clear();
}

//일부로 &안붙임 &해서 꺼내서 쓰다가 추가되면 안돼서
void InteractionManager::AllUpdateInteractionObject()
{
	LockGuard lock(m_lockInteraction);
	auto iter = m_lisInteraction.begin();
	while (iter != m_lisInteraction.end())
	{
		InteractionObject* object = *iter;
		object->Update();

		// 삭제해야될 오브젝트일경우 생명주기가 끝나거나 등등
		if (!object->GetValidLife())
		{
			// 해당 객체 삭제 리스트에 추가
			m_lisDeleteInteraction.push_back(*iter);

			// 이터레이터 없애기
			iter = m_lisInteraction.erase(iter);
			continue;
		}
		else
			++iter;
	}
}