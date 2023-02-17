#include "pch.h"
#include "ClientSession.h"

void ClientSession::PacketeHandle(BasePacket* _Packetdata)
{
	s_ClientSession serverSession = std::static_pointer_cast<ClientSession>(shared_from_this());

	ClientPacketHandler::RecievePacketHandling(serverSession, _Packetdata);
}

void ClientSession::LoginSuccess()
{
	m_Login = true;
}

bool ClientSession::DIdLogin()
{
	return m_Login;
}

void ClientSession::Chatting()
{
	if (!m_Login)
		return;
	SendBuffer* buffer = ClientPacketHandler::Send_Chatting();

	RegisterSend(buffer);
}

void ClientSession::Login()
{
	SendBuffer* buffer = ClientPacketHandler::Send_Login();

	RegisterSend(buffer);
}

void ClientSession::ChattingEnter()
{
	SendBuffer* buffer = ClientPacketHandler::Send_ChattingEnter();

	RegisterSend(buffer);
}

void ClientSession::DeleteSession()
{
}
