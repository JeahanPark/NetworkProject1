#include "pch.h"
#include "ClientSession.h"

void ClientSession::PacketeHandle(PacketData* _Packetdata)
{
	ClientPacketHandler::RecievePacketHandling(_Packetdata);
}

void ClientSession::Chatting()
{
	SendBuffer* buffer = ClientPacketHandler::Send_Chatting();

	RegisterSend(buffer);
}

void ClientSession::DeleteSession()
{
}
