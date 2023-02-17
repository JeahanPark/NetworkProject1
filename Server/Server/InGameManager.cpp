#include "pch.h"
#include "InGameManager.h"
#include "InGameObject.h"
#include "UserController.h"
InGameManager::~InGameManager()
{
    for (mapInGame::value_type inGameObject : m_mapInGame)
    {
        inGameObject.second = nullptr;
    }
    m_mapInGame.clear();
}

bool InGameManager::InsertInGameObject(s_ServerSession _session)
{
    LockGuard lock(m_lockInGame);
    
    if (m_iMaxInGameUser <= m_mapInGame.size())
        return false;

    int iUserIndex = _session->GetUserData()->GetUserIndex();

    // 이미 있다.
    if (m_mapInGame.find(_session->GetUserData()->GetUserIndex()) != m_mapInGame.end())
    {
        return false;
    }


    CreateInGameObject(_session);
	return true;
}

bool InGameManager::DeleteInGameObject(s_ServerSession _session)
{
    LockGuard lock(m_lockInGame);
    int iUserIndex = _session->GetUserData()->GetUserIndex();

    // 찾기
    auto iter = m_mapInGame.find(_session->GetUserData()->GetUserIndex());
    if (iter != m_mapInGame.end())
    {
        m_mapInGame.erase(iter);
        return true;
    }

	return false;
}

s_InGameObject InGameManager::GetInGameObject(int _iUserIndex)
{
    auto iter = m_mapInGame.find(_iUserIndex);
    if (iter != m_mapInGame.end())
    {
        return iter->second;
    }

    return nullptr;
}

void InGameManager::CreateInGameObject(s_ServerSession _session)
{
    s_UserController userController = make_shared<UserController>();

    s_InGameObject ingame = make_shared<InGameObject>(_session, userController);
    m_mapInGame.insert(std::make_pair(_session->GetUserData()->GetUserIndex(), ingame));

    //InteractionManager().GetInstance()->AddUserInteractionObject(_session->GetUserData()->GetUserIndex(), userController);
}
const mapInGame InGameManager::GetmapInGame()
{
    LockGuard lock(m_lockInGame);

    return m_mapInGame;
}

void InGameManager::GetlistInGame(list<s_InGameObject>& _lisInGame)
{
    LockGuard lock(m_lockInGame);

    for (mapInGame::value_type inGameObject : m_mapInGame)
    {
        if (inGameObject.second->GetUserInteraction() != nullptr)
            _lisInGame.push_back(inGameObject.second);
    }
}
