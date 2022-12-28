#include "pch.h"
#include "InGameUpdateManager.h"
#include "InGameObject.h"
#include <algorithm>

InGameUpdateManager::InGameUpdateManager() 
    : m_fDeltaTime(0.f), m_fSpt(0.f), m_fSptTime(0.f),  m_iFPS(0), m_fFPSTime(0.f)
{
    ZeroMemory(&m_CulTime, sizeof(LARGE_INTEGER));
    ZeroMemory(&m_OldTime, sizeof(LARGE_INTEGER));
    ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

InGameUpdateManager::~InGameUpdateManager()
{
}

void InGameUpdateManager::InGameUpdate()
{
    QueryPerformanceCounter(&m_CulTime);
    QueryPerformanceCounter(&m_OldTime);
    QueryPerformanceFrequency(&m_CpuTick);

    m_fSpt = 1.f / m_fFramPerSec;

    while (true)
    {
        // 초당 60프레임으로 해놓음 프레임 체크를 한다.
        if (FramCheck())
        {
            // 내부 interaction 업데이트
            InteractionManager::GetInstance()->AllUpdateInteractionObject();

            // 클라이언트 업데이트 패킷 보내기
            if (UpdateClientInteraction())
            {
                //모든 Interaction을 인게임에 들어온 세션들한테 패킷전달
                list<InteractionObject*> InteractionObjects = InteractionManager::GetInstance()->GetInteractionList();

                // 삭제할 interaction을 클라에 전달할 리스트에 추가한다.
                InteractionManager::GetInstance()->AddDeleteInteraction(InteractionObjects);

                const mapInGame inGames = InGameManager::GetInstance()->GetmapInGame();

                for (auto iter : inGames)
                {
                    const s_ServerSession session = iter.second->GetSession();
                    PacketHandler::InGameUpdate(InteractionObjects, session);
                }

                // 클라이언트한테 패킷을 다전달했으면 이때 다 삭제한다.
                InteractionManager::GetInstance()->ClearDeleteInteraction();
            }
        }
        InitFrame();
        // 프레임 렌더 테스트
    }

    //ULONGLONG saveTick = GetTickCount64();
    //while (true)
    //{
    //    double crttick = GetTickCount64() - saveTick;


    //    // 업데이트시작
    //    if (crttick / (double)CLOCKS_PER_SEC >= m_dInteractionUpdateSecond)
    //    {
    //        InteractionManager::GetInstance()->AllUpdateInteractionObject();
    //    }


    //    // 클라이언트 패킷 보내기
    //    if ((double)(crttick - saveClientUpdate) / CLOCKS_PER_SEC >= m_dClientUpdateSecond)
    //    {
    //        //모든 Interaction을 인게임에 들어온 세션들한테 패킷전달
    //        const list<InteractionObject*> InteractionObjects = InteractionManager::GetInstance()->GetInteractionList();

    //        const mapInGame inGames = InGameManager::GetInstance()->GetmapInGame();

    //        for (auto iter : inGames)
    //        {
    //            const s_ServerSession session = iter.second->GetSession();
    //            PacketHandler::InGameUpdate(InteractionObjects, session);
    //        }

    //        // 시간을 다시 세팅
    //        saveClientUpdate = GetTickCount64();
    //    }
    //}
}

bool InGameUpdateManager::UpdateClientInteraction()
{
    // 10프레임마다 갱신
    return m_iFPS % 10 == 0;
}


bool InGameUpdateManager::FramCheck()
{
    QueryPerformanceCounter(&m_CulTime);
    QueryPerformanceFrequency(&m_CpuTick);

    m_fDeltaTime = float(m_CulTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;
    m_OldTime = m_CulTime;
    m_fSptTime += m_fDeltaTime;

    if (m_fSpt < m_fSptTime)
    {
        ++m_iFPS;
        m_fSptTime = 0.f;
        return true;
    }
    return false;
}

void InGameUpdateManager::InitFrame()
{
    m_fFPSTime += m_fDeltaTime;

    if (1.f < m_fFPSTime)
    {
        //cout << "FPS : " << m_iFPS << endl;
        m_iFPS = 0;
        m_fFPSTime = 0.f;
    }
}
