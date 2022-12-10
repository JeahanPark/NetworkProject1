#include "pch.h"
#include "InGameUpdateManager.h"
#include "InGameObject.h"

InGameUpdateManager::InGameUpdateManager()
{
}

InGameUpdateManager::~InGameUpdateManager()
{
}

void InGameUpdateManager::InGameUpdate()
{
    ULONGLONG saveTick = GetTickCount64();
    while (true)
    {
        ULONGLONG crttick = GetTickCount64();

        // 업데이트 시간이 안됐다.
        if ((double)(crttick - saveTick) / CLOCKS_PER_SEC < m_dUpdateCycleSecond)
            continue;

        saveTick = GetTickCount64();

        // 업데이트시작

        //모든 Interaction Update세팅
        const list<InteractionObject*> InteractionObjects = InteractionManager::GetInstance()->AllUpdateInteractionObject();


        //모든 Collison Update세팅
        {

        }
        //모든 Interaction을 인게임에 들어온 세션들한테 패킷전달
        
        const mapInGame inGames = InGameManager::GetInstance()->GetmapInGame();

        for (auto iter : inGames)
        {
            const s_ServerSession session = iter.second->GetSession();
            PacketHandler::InGameUpdate(InteractionObjects, session);
        }
    }
}
