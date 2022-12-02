#include "pch.h"
#include "ChattingManager.h"
#include "ChattingObject.h"

ChattingManager::~ChattingManager()
{
    for (ChattingObject* chatting : m_lisChatting)
    {
        delete chatting;
    }
    m_lisChatting.clear();
}

bool ChattingManager::IsExistentChattingObject(s_ServerSession _session)
{
    int iUserIndex = _session->GetUserData()->GetUserIndex();

    // 있다.
    for (ChattingObject* chatting : m_lisChatting)
    {
        if (chatting->SameSession(_session->GetUserData()->GetUserIndex()))
            return true;
    }
    return false;
}

bool ChattingManager::InsertChattingObject(s_ServerSession _session)
{
    LockGuard lock(m_lockChatting);

    if (m_iMaxChattingUser <= m_lisChatting.size())
        return false;
   
    int iUserIndex = _session->GetUserData()->GetUserIndex();

    // 이미 있다.
    for (ChattingObject* chatting : m_lisChatting)
    {
        if (chatting->SameSession(_session->GetUserData()->GetUserIndex()))
            return false;
    }

    ChattingObject* chatting = new ChattingObject(_session);
    m_lisChatting.push_back(chatting);
    return true;
}

bool ChattingManager::DeleteChattingObject(s_ServerSession _session)
{
    LockGuard lock(m_lockChatting);

    int iUserIndex = _session->GetUserData()->GetUserIndex();

    // 찾기
    for (ChattingObject* chatting : m_lisChatting)
    {
        if (chatting->SameSession(_session->GetUserData()->GetUserIndex()))
        {
            m_lisChatting.remove(chatting);
            return true;
        }
    }
    return false;
}


bool ChattingManager::AllSendChatting(ChattingPacket* packetData)
{
    auto iter = m_lisChatting.begin();
    while (iter != m_lisChatting.end())
    {
        if (!(*iter)->UseChatting())
        {
            // 채팅 불가능 삭제
            auto delteIter = iter++;
            delete (*delteIter);
            m_lisChatting.erase(delteIter);
        }
        else
        {
            (*iter)->SendChatting(packetData);
            iter++;
        }
    }

    return false;
}
