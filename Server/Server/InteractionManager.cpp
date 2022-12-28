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

//�Ϻη� &�Ⱥ��� &�ؼ� ������ ���ٰ� �߰��Ǹ� �ȵż�
void InteractionManager::AllUpdateInteractionObject()
{
	LockGuard lock(m_lockInteraction);
	auto iter = m_lisInteraction.begin();
	while (iter != m_lisInteraction.end())
	{
		InteractionObject* object = *iter;
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