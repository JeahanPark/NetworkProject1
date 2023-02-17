#include "pch.h"
#include "ServerSession.h"


void ServerSession::SetUserData(WCHAR* _userID, int _iScore, int _iUserIndex)
{
	m_userData = new UserData(_userID, _iScore, _iUserIndex);
}

UserData* ServerSession::GetUserData()
{
	return m_userData;
}

void ServerSession::PacketeHandle(BasePacket* _Packetdata)
{
	s_ServerSession serverSession = std::static_pointer_cast<ServerSession>(shared_from_this());

	PacketHandler::PacketHandling(serverSession, _Packetdata);
}

void ServerSession::DeleteSession()
{
	s_ServerSession serverSession = std::static_pointer_cast<ServerSession>(shared_from_this());

	ServerSessionManager().GetInstance()->DeleteSession(serverSession);


	if (m_userData != nullptr)
	{
		InGameManager::GetInstance()->DeleteInGameObject(serverSession);
		delete m_userData;
		m_userData = nullptr;
	}
}
