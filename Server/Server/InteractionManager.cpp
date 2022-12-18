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
	auto iter = m_lisInteraction.begin();
	while (iter != m_lisInteraction.end())
	{
		InteractionObject* object = *iter;
		object->Update();

		// �����ؾߵ� ������Ʈ�ϰ�� �����ֱⰡ �����ų� ���
		if (!object->GetValidLife())
		{
			// �ش� ��ü ����
			InteractionObject* object = *iter;
			delete object;

			// ���ͷ����� ���ֱ�
			iter = m_lisInteraction.erase(iter);
			continue;
		}
		else
			++iter;
	}
	//for (auto iter = m_lisInteraction.begin(); iter != m_lisInteraction.end(); ++iter)
	//{
	//	InteractionObject* object = *iter;
	//	object->Update();

	//	// �����ؾߵ� ������Ʈ�ϰ�� �����ֱⰡ �����ų� ���
	//	if (!object->GetValidLife())
	//	{
	//		// �ش� ��ü ����
	//		InteractionObject* object = *iter;
	//		delete object;

	//		// ���ͷ����� ���ֱ�
	//		iter = m_lisInteraction.erase(iter);
	//		continue;
	//	}
	//}

	return m_lisInteraction;
}