#include "pch.h"
#include "ContentsManager.h"
#include "InteractionObject.h"
#include "AttackDummy.h"
#include "UserObject.h"

ContentsManager::ContentsManager() : m_fReflectionCreateCrtTime(0.f)
{

}

void ContentsManager::InitContents()
{
	CreateDummy();

}
void ContentsManager::UpdateFieldItem()
{
	m_fReflectionCreateCrtTime += InGameUpdateManager::GetInstance()->GetDeltaTime();

	if (m_fReflectionCreateCrtTime >= m_fReflectionCreateMaxTime)
	{
		int iReflectionMaxCount = m_iReflectionItemMaxCount.load();
		int iReflectionCrtCount = m_iReflectionItemCrtCount.load();

		if (iReflectionCrtCount < iReflectionMaxCount)
		{
			m_iReflectionItemCrtCount.fetch_add(1);

			CreateReflectionItem();
			m_fReflectionCreateCrtTime = 0;
		}
	}
}
void ContentsManager::UpdateReflectionMaxCount(int _iCount)
{
	m_iReflectionItemMaxCount.fetch_add(_iCount);
}
void ContentsManager::UpdateReflectionCrtCount(int _iCount)
{
	m_iReflectionItemCrtCount.fetch_add(_iCount);
}
void ContentsManager::CreateDummy()
{
	list<s_InteractionObejct> lisInteraction;

	XMFLOAT3 startPos = { 10,0,0 };
	s_InteractionObejct interaction = InteractionCreator::CreateDummyInteraction(startPos);
	lisInteraction.push_back(interaction);

	startPos = { -10,0,0 };
	interaction = InteractionCreator::CreateDummyInteraction(startPos);
	lisInteraction.push_back(interaction);

	InteractionManager::GetInstance()->AddListInteractionObejct(lisInteraction);
}

void ContentsManager::CreateReflectionItem()
{
	float x = rand() % (iMaxMapSizeX * 2) - iMaxMapSizeX;
	float z = rand() % (iMaxMapSizeY * 2) - iMaxMapSizeY;

	XMFLOAT3 startPos = { x,0,z };

	s_InteractionObejct interaction = InteractionCreator::CreateReflectionItem(startPos);

	InteractionManager::GetInstance()->AddInteractionObject(interaction);
}
