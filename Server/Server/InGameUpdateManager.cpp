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
        // �ʴ� 60���������� �س��� ������ üũ�� �Ѵ�.
        if (FramCheck())
        {
            // ���� interaction ������Ʈ
            InteractionManager::GetInstance()->AllUpdateInteractionObject();

            // Ŭ���̾�Ʈ ������Ʈ ��Ŷ ������
            if (UpdateClientInteraction())
            {
                //��� Interaction�� �ΰ��ӿ� ���� ���ǵ����� ��Ŷ����
                list<InteractionObject*> InteractionObjects = InteractionManager::GetInstance()->GetInteractionList();

                // ������ interaction�� Ŭ�� ������ ����Ʈ�� �߰��Ѵ�.
                InteractionManager::GetInstance()->AddDeleteInteraction(InteractionObjects);

                const mapInGame inGames = InGameManager::GetInstance()->GetmapInGame();

                for (auto iter : inGames)
                {
                    const s_ServerSession session = iter.second->GetSession();
                    PacketHandler::InGameUpdate(InteractionObjects, session);
                }

                // Ŭ���̾�Ʈ���� ��Ŷ�� ������������ �̶� �� �����Ѵ�.
                InteractionManager::GetInstance()->ClearDeleteInteraction();
            }
        }
        InitFrame();
        // ������ ���� �׽�Ʈ
    }

    //ULONGLONG saveTick = GetTickCount64();
    //while (true)
    //{
    //    double crttick = GetTickCount64() - saveTick;


    //    // ������Ʈ����
    //    if (crttick / (double)CLOCKS_PER_SEC >= m_dInteractionUpdateSecond)
    //    {
    //        InteractionManager::GetInstance()->AllUpdateInteractionObject();
    //    }


    //    // Ŭ���̾�Ʈ ��Ŷ ������
    //    if ((double)(crttick - saveClientUpdate) / CLOCKS_PER_SEC >= m_dClientUpdateSecond)
    //    {
    //        //��� Interaction�� �ΰ��ӿ� ���� ���ǵ����� ��Ŷ����
    //        const list<InteractionObject*> InteractionObjects = InteractionManager::GetInstance()->GetInteractionList();

    //        const mapInGame inGames = InGameManager::GetInstance()->GetmapInGame();

    //        for (auto iter : inGames)
    //        {
    //            const s_ServerSession session = iter.second->GetSession();
    //            PacketHandler::InGameUpdate(InteractionObjects, session);
    //        }

    //        // �ð��� �ٽ� ����
    //        saveClientUpdate = GetTickCount64();
    //    }
    //}
}

bool InGameUpdateManager::UpdateClientInteraction()
{
    // 10�����Ӹ��� ����
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
