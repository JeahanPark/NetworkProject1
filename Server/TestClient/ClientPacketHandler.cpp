#include "pch.h"
#include "ClientPacketHandler.h"

using namespace std;

void ClientPacketHandler::RecievePacketHandling(PacketData* _Packetdata)
{
	switch (_Packetdata->m_PakcetType)
	{
	case ePacketType::SToC_Chatting:
		Recieve_Chatting((ChattingPacket*)_Packetdata);
		break;
	case ePacketType::CToS_Chatting:
		//cout << "Client_Send_Chatting" << endl;
		break;
	default:
		break;
	}
}

void ClientPacketHandler::Recieve_Chatting(ChattingPacket* _chattingPacket)
{
	cout << "receiveData : " << _chattingPacket->m_chattingContent << endl;
}

SendBuffer* ClientPacketHandler::Send_Chatting()
{
	WCHAR chattingContent[300] = L"qweqwe123";

	SendBuffer* pSendBuffer = PacketCreate::ChattingPacketCreate(chattingContent, ePacketType::CToS_Chatting);

	return pSendBuffer;
}
