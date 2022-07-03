#include "ClientPacketHandler.h"
#include <iostream>
#include <string.h>
using namespace std;

void ClientPacketHandler::RecievePacketHandling(char* _buffer)
{
	PacketData* data = (PacketData*)_buffer;


	switch (data->m_PakcetType)
	{
	case PacketType::SToC_Chatting:
		Recieve_Chatting((ChattingPacket*)data);
		break;
	default:
		break;
	}
}

void ClientPacketHandler::Recieve_Chatting(ChattingPacket* _chattingPacket)
{
	cout << "receiveData : " << _chattingPacket->chattingContent << endl;
}

char* ClientPacketHandler::Send_Chatting()
{
	ChattingPacket* chatting = new ChattingPacket();
	chatting->m_PakcetType = PacketType::SToC_Chatting;
	chatting->m_iSize = sizeof(ChattingPacket);
	strcat_s(chatting->chattingContent, "qweqwe123");

	return (char*)chatting;
}
