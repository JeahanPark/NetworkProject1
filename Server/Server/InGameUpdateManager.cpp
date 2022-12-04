#include "pch.h"
#include "InGameUpdateManager.h"
#include "InGameObject.h"

InGameUpdateManager::InGameUpdateManager()
{
}

InGameUpdateManager::~InGameUpdateManager()
{
}

void InGameUpdateManager::InGameUpdateStart()
{
    g_ThreadManager->Run([]
    {
        InGameUpdateManager::GetInstance()-> InGameUpdate();
    });
}

void InGameUpdateManager::InGameUpdate()
{
    ULONGLONG saveTick = GetTickCount64();
    while (true)
    {
        ULONGLONG crttick = GetTickCount64();

        // ������Ʈ �ð��� �ȵƴ�.
        if ((double)(crttick - saveTick) / CLOCKS_PER_SEC < m_dUpdateCycleSecond)
            continue;

        // ������Ʈ����

        //��� Interaction Update����
        InteractionManager::GetInstance()->AllUpdateInteractionObject();


        //��� Collison Update����
        {

        }
        //��� Interaction�� �ΰ��ӿ� ���� ���ǵ����� ��Ŷ����
        {
            const list<InteractionObject*>& InteractionObjects = InteractionManager::GetInstance()->GetlisInteractionObject();

            const mapInGame inGames = InGameManager::GetInstance()->GetmapInGame();

            for (auto iter : inGames)
            {
                const s_ServerSession session = iter.second->GetSession();
                PacketHandler::InGameUpdate(InteractionObjects, session);
            }
        }


    }
}
