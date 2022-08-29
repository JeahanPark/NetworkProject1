#include "pch.h"
#include "ServerSession.h"


UserData* ServerSession::GetUserData()
{
	return &m_userData;
}

void ServerSession::PacketeHandle(PacketData* _Packetdata)
{
	s_ServerSession serverSession = std::static_pointer_cast<ServerSession>(shared_from_this());

	PacketHandler::PacketHandling(serverSession, _Packetdata);
}
