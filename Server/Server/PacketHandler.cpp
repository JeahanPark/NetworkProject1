#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::PacketHandling(PacketData* _Packetdata)
{
	switch (_Packetdata->m_PakcetType)
	{
	case PacketType::CToS_Login:
		break;

	case PacketType::Both_Chatting:
		Chatting((ChattingPacket*)_Packetdata);
		break;

	default:
		break;
	}
}

void PacketHandler::Chatting(ChattingPacket* _Packetdata)
{
	std::vector<Session*> vecSession = g_SessionManager->GetSessions();

	int nCount = (int)vecSession.size();
	for (int i = 0; i < nCount; ++i)
	{
		SocketEvent* sEvent = new SocketEvent(SocketEventType::SocketEventType_Send, vecSession[i]);


		ChattingPacket chatting;
		chatting.m_PakcetType = PacketType::Both_Chatting;
		chatting.m_iSize = sizeof(ChattingPacket);

		WSABUF wsaBuf;
		wsaBuf.buf = _Packetdata->chattingContent;
		wsaBuf.len = BUFSIZE;

		DWORD sendLen = 0;
		DWORD flags = 0;

		WSASend(vecSession[i]->GetSocket(), &wsaBuf, 1, &sendLen, flags, (LPWSAOVERLAPPED)sEvent, NULL);
	}

}
