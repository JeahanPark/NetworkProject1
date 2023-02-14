#include "pch.h"
#include "ContentsManager.h"
#include "InteractionObject.h"
#include "AttackDummy.h"
void ContentsManager::InitContents()
{
	CreateDummy();

}
void ContentsManager::CreateDummy()
{
	list<s_InteractionObejct> lisInteraction;

	XMFLOAT3 startPos = { 10,0,0 };
	s_InteractionObejct interaction = InteractionManager::CreateDummyInteraction(startPos);
	lisInteraction.push_back(interaction);

	startPos = { -10,0,0 };
	interaction = InteractionManager::CreateDummyInteraction(startPos);
	lisInteraction.push_back(interaction);

	InteractionManager::GetInstance()->AddListInteractionObejct(lisInteraction);
}
