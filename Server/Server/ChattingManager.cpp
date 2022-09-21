#include "pch.h"
#include "ChattingManager.h"
#include "ChattingObject.h"

ChattingManager::~ChattingManager()
{
    for (ChattingObject* chatting : m_setChatting)
    {
        delete chatting;
    }
    m_setChatting.clear();
}

void ChattingManager::InsertChattingObject(s_ServerSession _session)
{
    LockGuard lock(m_lockChatting);

    ChattingObject* chatting = new ChattingObject(_session);
    m_setChatting.insert(chatting);
}

void ChattingManager::DeleteChattingObject(s_ServerSession _session)
{
    LockGuard lock(m_lockChatting);

    m_setChatting.erase(_chatting);
}

set<ChattingObject*> ChattingManager::GetChattingObjects()
{
    return m_setChatting;
}
