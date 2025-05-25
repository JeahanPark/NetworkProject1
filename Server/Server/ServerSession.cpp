#include "pch.h"
#include "ServerSession.h"


ServerSession::ServerSession()
{
	static int index;
	WCHAR tempID[USER_ID_LENGTH];
	swprintf_s(tempID, USER_ID_LENGTH, L"temp%d", index);

	SetUserData(tempID, 0, index);

	index++;
}

void ServerSession::InitSession(HANDLE _iocpHandle, SOCKET _socket)
{
	Session::InitSession(_iocpHandle, _socket);

	ePacketResult packetResult = ePacketResult::Success;

	s_ServerSession serverSession = std::static_pointer_cast<ServerSession>(shared_from_this());

	SendBuffer* pSendBuffer = PacketHandler::LoginResultPacketCreate(serverSession, packetResult);
	serverSession->RegisterSend(pSendBuffer);
}

void ServerSession::SetUserData(const WCHAR* _userID, int _iScore, int _iUserIndex)
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
