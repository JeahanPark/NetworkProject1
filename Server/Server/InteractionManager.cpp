#include "pch.h"
#include "InteractionManager.h"
#include "InteractionObject.h"
#include "Collision.h"

#include "UserObject.h"
#include "AttackDummy.h"

InteractionManager::~InteractionManager()
{
}

s_InteractionObejct InteractionManager::CreateUserInteraction(s_UserController _userController, const UserData* _userData)
{
	s_InteractionObejct interaction = make_shared<UserObject>(_userController, _userData);
	interaction->Init();

	return interaction;
}

s_InteractionObejct InteractionManager::CreateDummyInteraction(const XMFLOAT3& _startPos)
{
	s_InteractionObejct interaction = make_shared<AttackDummy>(_startPos);
	interaction->Init();

	return interaction;
}

void InteractionManager::AddInteractionObject(s_InteractionObejct _interaction)
{
	LockGuard lock(m_lockInteraction);
	
	eInteractionType type = _interaction->GetInteractionType();

	auto find = m_mapInteraction.find(type);

	if (find == m_mapInteraction.end())
	{
		//����
		m_mapInteraction.insert(std::make_pair(type, lisInteraction()));
	}

	m_mapInteraction.find(type)->second.push_back(_interaction);

	//m_lisInteraction.push_back(_interaction);
}

void InteractionManager::AddListInteractionObejct(const list<s_InteractionObejct>& _lisInteraction)
{
	LockGuard lock(m_lockInteraction);
	for (auto iter : _lisInteraction)
	{
		eInteractionType type = iter->GetInteractionType();

		auto find = m_mapInteraction.find(type);

		if (find == m_mapInteraction.end())
		{
			//����
			m_mapInteraction.insert(std::make_pair(type, lisInteraction()));
		}

		m_mapInteraction.find(type)->second.push_back(iter);
	}
	
}

void InteractionManager::GetInteractionList(list<s_InteractionObejct>& _InteractionObjects)
{
	LockGuard lock(m_lockInteraction);

	GetNoLockInteractionList(_InteractionObjects);
}

void InteractionManager::GetInteractionTypeList(eInteractionType _type, lisInteraction& _lisInteraction)
{
	LockGuard lock(m_lockInteraction);

	GetNoLockInteractionTypeList(_type, _lisInteraction);
}


void InteractionManager::GetDeleteInteraction(list<s_InteractionObejct>& _InteractionObjects)
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
	
	list<s_InteractionObejct> _InteractionObjects;
	GetNoLockInteractionList(_InteractionObjects);

	// �浹 �˻��ϱ�
	{
		for (auto type : m_mapInteraction)
		{
			auto iter = type.second.begin();

			while (iter != type.second.end())
			{
				s_InteractionObejct object = *iter;

				object->GetCollision()->Update(_InteractionObjects);

				++iter;
			}
		}
	}

	
	// ���ͷ��� Update�Ұ͵� �ϱ�
	{
		for (auto& type : m_mapInteraction)
		{
			lisInteraction& list = type.second;

			auto iter = list.begin();

			while (iter != list.end())
			{
				s_InteractionObejct object = *iter;
				object->Update();

				// �����ؾߵ� ������Ʈ�ϰ�� �����ֱⰡ �����ų� ���
				if (!object->GetValidLife())
				{
					// �ش� ��ü ���� ����Ʈ�� �߰�
					m_lisDeleteInteraction.push_back(*iter);

					// ���ͷ����� ���ֱ�
					iter = list.erase(iter);
					continue;
				}
				else
					++iter;
			}
		}
	}
}

s_InteractionObejct InteractionManager::GetUser()
{
	lisInteraction lisInteraction;
	{
		LockGuard lock(m_lockInteraction);

		GetNoLockInteractionTypeList(eInteractionType::User, lisInteraction);
	}

	for ( auto iter : lisInteraction)
	{
		//UserObject* user = (UserObject*)iter.get;
	}

	return s_InteractionObejct();
}

void InteractionManager::GetNoLockInteractionList(list<s_InteractionObejct>& _InteractionObjects)
{
	for (auto type : m_mapInteraction)
	{
		auto iter = type.second.begin();

		while (iter != type.second.end())
		{
			_InteractionObjects.push_back(*iter);
			++iter;
		}
	}
}

void InteractionManager::GetNoLockInteractionTypeList(eInteractionType _type, lisInteraction& _lisInteraction)
{
	auto iter = m_mapInteraction.find(_type);

	if (iter == m_mapInteraction.end())
		return;

	for (auto interaction : iter->second)
	{
		_lisInteraction.push_back(interaction);
	}
}
