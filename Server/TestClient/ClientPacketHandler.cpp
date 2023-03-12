#include "pch.h"
#include "ClientPacketHandler.h"

using namespace std;

void ClientPacketHandler::RecievePacketHandling(s_ClientSession _session, BasePacket* _packetData)
{
	switch (_packetData->m_PakcetType)
	{
	case ePacketType::Signal:
		PacketSignal(_session, _packetData);
		break;
	case ePacketType::SToC_LoginResult:
		RecieveLogin(_session);
		break;
	case ePacketType::SToC_Chatting:
		Recieve_Chatting((ChattingPacket*)_packetData);
		break;
	case ePacketType::CToS_Chatting:
		//cout << "Client_Send_Chatting" << endl;
		break;
	default:
		break;
	}
}

void ClientPacketHandler::RecieveLogin(s_ClientSession _session)
{
	//wcout << "ChattingEnter" << endl;
	_session->LoginSuccess();
	_session->ChattingEnter();
}

void ClientPacketHandler::Recieve_Chatting(ChattingPacket* _chattingPacket)
{
	//wcout << "receiveData : " << _chattingPacket->m_chattingContent << endl;
}

void ClientPacketHandler::PacketSignal(s_ClientSession _session, BasePacket* _packetData)
{
	SignalPacket* packetSignal = (SignalPacket*)_packetData;

	switch (packetSignal->m_ePacketSignal)
	{
	case ePacketSignal::Signal_ChattingRoomEnter:
			//wcout << "receiveData : Signal_ChattingRoomEnter" << endl;
			break;
	}
}

SendBuffer* ClientPacketHandler::Send_Chatting()
{
	WCHAR chattingContent[CHATTING_LENGTH] = L"qweqwe123";

	SendBuffer* pSendBuffer = PacketCreate::ChattingPacketCreate(chattingContent, ePacketType::CToS_Chatting);

	return pSendBuffer;
}

SendBuffer* ClientPacketHandler::Send_Login()
{
	SendBuffer* pSendBuffer = new SendBuffer(sizeof(LoginRequestPacket));

	LoginRequestPacket* login = (LoginRequestPacket*)pSendBuffer->GetSendBufferAdress();
	
	login->m_PakcetType = ePacketType::CToS_Login;
	login->m_iSize = sizeof(LoginRequestPacket);
	wcscpy_s(login->m_UserID, L"servertest01");
	wcscpy_s(login->m_Password, L"123123");

	return pSendBuffer;
}

SendBuffer* ClientPacketHandler::Send_ChattingEnter()
{
	SendBuffer* pSendBuffer = new SendBuffer(sizeof(SignalPacket));

	SignalPacket* login = (SignalPacket*)pSendBuffer->GetSendBufferAdress();

	login->m_PakcetType = ePacketType::Signal;
	login->m_ePacketSignal = ePacketSignal::Signal_ChattingRoomEnter;

	login->m_iSize = sizeof(SignalPacket);

	return pSendBuffer;
}
