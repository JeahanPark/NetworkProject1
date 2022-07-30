#include "pch.h"
#include "ServerSession.h"

void ServerSession::PacketeHandle(PacketData* _Packetdata)
{
	PacketHandler::PacketHandling(shared_from_this(), _Packetdata);
}
