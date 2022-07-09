#include "pch.h"
#include "ClientSession.h"

void ClientSession::PacketeHandle(PacketData* _Packetdata)
{
	ClientPacketHandler::RecievePacketHandling(_Packetdata);
}

void ClientSession::Chetting()
{
	SendBuffer* buffer = ClientPacketHandler::Send_Chatting();

	RegisterSend(buffer);
}
