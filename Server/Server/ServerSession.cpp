#include "pch.h"
#include "ServerSession.h"

void ServerSession::PacketeHandle(PacketData* _Packetdata)
{
	PacketHandler::PacketHandling(this, _Packetdata);
}
