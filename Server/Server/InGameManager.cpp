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

bool InGameManager::InsertInGameObject(s_InGameObject _inGameObject)
{
    LockGuard lock(m_lockInGame);
    
    if (m_iMaxInGameUser <= m_mapInGame.size())
        return false;

    int userIndex = _inGameObject->GetSession()->GetUserData()->GetUserIndex();

    // 이미 있다.
    if (m_mapInGame.find(userIndex) != m_mapInGame.end())
    {
        return false;
    }

    m_mapInGame.insert(std::make_pair(userIndex, _inGameObject));

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
    LockGuard lock(m_lockInGame);

    auto iter = m_mapInGame.find(_iUserIndex);
    if (iter != m_mapInGame.end())
    {
        return iter->second;
    }

    return nullptr;
}

s_InGameObject InGameManager::CreateInGameObject(s_ServerSession _session)
{
    s_UserController userController = make_shared<UserController>();

    s_InGameObject ingame = make_shared<InGameObject>(_session, userController);
    //m_mapInGame.insert(std::make_pair(_session->GetUserData()->GetUserIndex(), ingame));

    //InteractionManager().GetInstance()->AddUserInteractionObject(_session->GetUserData()->GetUserIndex(), userController);

    return ingame;
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
        _lisInGame.push_back(inGameObject.second);
    }
}

bool InGameManager::ExistInGameObject(int _iUserIndex)
{
    LockGuard lock(m_lockInGame);

    return m_mapInGame.find(_iUserIndex) != m_mapInGame.end();
}
