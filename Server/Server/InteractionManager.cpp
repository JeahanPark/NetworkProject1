#include "pch.h"
#include "InteractionManager.h"
#include "InteractionObject.h"
#include "UserObject.h"
InteractionManager::~InteractionManager()
{
	m_lisInteraction.clear();
}

s_InteractionObejct InteractionManager::CreateUserInteraction(s_UserController _userController, const UserData* _userData)
{
	s_InteractionObejct interaction = make_shared<UserObject>(_userController, _userData);

	return interaction;
}

void InteractionManager::AddInteractionObject(s_InteractionObejct _interaction)
{
	LockGuard lock(m_lockInteraction);

	m_lisInteraction.push_back(_interaction);
}

void InteractionManager::GetUserInteractionList(list<s_InteractionObejct>& _lisUserInteracction)
{
	LockGuard lock(m_lockInteraction);

	for (auto iter : m_lisInteraction)
	{
		if (iter->GetInteractionType() == eInteractionType::User)
		{
			_lisUserInteracction.push_back(iter);
		}
	}
	
}

void InteractionManager::AddDeleteInteraction(list<s_InteractionObejct>& _InteractionObjects)
{
	for (auto iter : m_lisDeleteInteraction)
	{
		_InteractionObjects.push_back(iter);
	}
}

void InteractionManager::ClearDeleteInteraction()
{
	m_lisDeleteInteraction.clear();
}

void InteractionManager::AllUpdateInteractionObject()
{
	LockGuard lock(m_lockInteraction);
	auto iter = m_lisInteraction.begin();
	while (iter != m_lisInteraction.end())
	{
		s_InteractionObejct object = *iter;
		object->Update();

		// �����ؾߵ� ������Ʈ�ϰ�� �����ֱⰡ �����ų� ���
		if (!object->GetValidLife())
		{
			// �ش� ��ü ���� ����Ʈ�� �߰�
			m_lisDeleteInteraction.push_back(*iter);

			// ���ͷ����� ���ֱ�
			iter = m_lisInteraction.erase(iter);
			continue;
		}
		else
			++iter;
	}
}