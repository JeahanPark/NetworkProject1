#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::PacketHandling(Session* _session, PacketData* _Packetdata)
{
	switch (_Packetdata->m_PakcetType)
	{
	case PacketType::CToS_Login:
		break;

	case PacketType::CToS_Chatting:
		Chatting((ChattingPacket*)_Packetdata);
		cout << _session->GetSessionNumber() << " : " << "Receive_Chatting" << endl;
		break;
	case PacketType::SToC_Chatting:
		cout << _session->GetSessionNumber() << " : " << "Send_Chatting" << endl;
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
		SendBuffer* pSendBuffer = PacketCreate::ChattingPacketCreate(_Packetdata->chattingContent, PacketType::SToC_Chatting);

		vecSession[i]->RegisterSend(pSendBuffer);
	}
}
