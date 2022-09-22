#include "pch.h"
#include "ChattingObject.h"

ChattingObject::ChattingObject(s_ServerSession _session) : m_session(_session)
{
}

ChattingObject::~ChattingObject()
{
	m_session = nullptr;
}

bool ChattingObject::SameSession(int _iUserIndex)
{
	UserData* userData = m_session->GetUserData();
	if (userData == nullptr)
		return false;

	return userData->GetUserIndex() == _iUserIndex;
}

bool ChattingObject::UseChatting()
{
	return m_session->IsLogin();
}

void ChattingObject::SendChatting(ChattingPacket* packetData)
{
	SendBuffer* pSendBuffer = PacketCreate::ChattingPacketCreate(packetData->m_chattingContent, ePacketType::SToC_Chatting);
	m_session->RegisterSend(pSendBuffer);
}
