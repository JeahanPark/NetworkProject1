#include "pch.h"
#include "PacketHandler.h"
#include "SendBuffer.h"

void PacketHandler::PacketHandling(Session* _session, PacketData* _Packetdata)
{
	switch (_Packetdata->m_PakcetType)
	{
	case PacketType::CToS_Login:
		break;

	case PacketType::CToS_Chatting:
		Chatting((ChattingPacket*)_Packetdata);
		break;
	case PacketType::SToC_Chatting:
		cout << _session->GetSessionNumber() << " : " << "SToC_Chatting" << endl;
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
		SendBuffer* pSendBuffer = new SendBuffer(sizeof(ChattingPacket));

		ChattingPacket* chatting = (ChattingPacket*)pSendBuffer->GetSendBufferAdress();
		chatting->m_PakcetType = PacketType::SToC_Chatting;
		chatting->m_iSize = sizeof(ChattingPacket);
		strcpy_s(chatting->chattingContent, _Packetdata->chattingContent);

		pSendBuffer->WsaBufSetting();

		vecSession[i]->RegisterSend(pSendBuffer);
	}

}
