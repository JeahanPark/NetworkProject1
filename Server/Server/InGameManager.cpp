#include "pch.h"
#include "InGameManager.h"
#include "InGameObject.h"
InGameManager::~InGameManager()
{
    for (InGameObject* chatting : m_lisInGame)
    {
        delete chatting;
    }
    m_lisInGame.clear();
}

bool InGameManager::InsertChattingObject(s_ServerSession _session)
{
    LockGuard lock(m_lockInGame);
    
    if (m_iMaxInGameUser <= m_lisInGame.size())
        return false;

    int iUserIndex = _session->GetUserData()->GetUserIndex();

    // 이미 있다.
    for (InGameObject* ingame : m_lisInGame)
    {
        if (ingame->SameSession(_session->GetUserData()->GetUserIndex()))
            return false;
    }

    InGameObject* ingame = new InGameObject(_session);
    m_lisInGame.push_back(ingame);
	return true;
}

bool InGameManager::DeleteChattingObject(s_ServerSession _session)
{
    LockGuard lock(m_lockInGame);
    int iUserIndex = _session->GetUserData()->GetUserIndex();

    // 찾기
    for (InGameObject* ingame : m_lisInGame)
    {
        if (ingame->SameSession(_session->GetUserData()->GetUserIndex()))
        {
            m_lisInGame.remove(ingame);
            return true;
        }
    }
	return false;
}
