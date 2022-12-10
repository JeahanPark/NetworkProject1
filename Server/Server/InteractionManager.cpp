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

//�Ϻη� &�Ⱥ��� &�ؼ� ������ ���ٰ� �߰��Ǹ� �ȵż�
list<InteractionObject*> InteractionManager::AllUpdateInteractionObject()
{
	LockGuard lock(m_lockInteraction);

	for (auto iter = m_lisInteraction.begin(); iter != m_lisInteraction.end(); ++iter)
	{
		InteractionObject* object = *iter;
		object->Update();

		// �����ؾߵ� ������Ʈ�ϰ�� �����ֱⰡ �����ų� ���
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