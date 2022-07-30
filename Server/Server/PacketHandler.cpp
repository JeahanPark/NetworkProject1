#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::PacketHandling(shared_session _session, PacketData* _Packetdata)
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
	set<shared_session> vecSession = g_SessionManager->GetSessions();

	for (shared_session iter : vecSession)
	{
		SendBuffer* pSendBuffer = PacketCreate::ChattingPacketCreate(_Packetdata->chattingContent, PacketType::SToC_Chatting);

		iter->RegisterSend(pSendBuffer);
	}
}
