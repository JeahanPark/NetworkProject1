#include "pch.h"
#include "ClientPacketHandler.h"

using namespace std;

void ClientPacketHandler::RecievePacketHandling(PacketData* _Packetdata)
{
	switch (_Packetdata->m_PakcetType)
	{
	case PacketType::SToC_Chatting:
		Recieve_Chatting((ChattingPacket*)_Packetdata);
		break;
	case PacketType::CToS_Chatting:
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
	char chattingContent[500] = "qweqwe123";

	SendBuffer* pSendBuffer = PacketCreate::ChattingPacketCreate(chattingContent, PacketType::CToS_Chatting);

	return pSendBuffer;
}
