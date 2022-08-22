#include "pch.h"
#include "PacketHandler.h"

#define SESSION_LOG(SessionNumber, LogName) cout << SessionNumber << " , " << LogName << endl;

void PacketHandler::PacketHandling(shared_session _session, PacketData* _packetData)
{
	switch (_packetData->m_PakcetType)
	{
	case ePacketType::CToS_Login:
		break;

	case ePacketType::CToS_Chatting:
		Chatting(_packetData);
		SESSION_LOG(_session->GetSessionNumber(), "CToS_Chatting")
		break;
	case ePacketType::CToS_UserRegister:
		Register(_session, _packetData);
		SESSION_LOG(_session->GetSessionNumber(), "CToS_UserRegister")
			break;



	case ePacketType::SToC_Chatting:
		SESSION_LOG(_session->GetSessionNumber(), "SToC_Chatting")
		break;
	case ePacketType::SToC_PacketResult:
		SESSION_LOG(_session->GetSessionNumber(), "SToC_PacketResult")
		break;
	default:
		break;
	}
}

void PacketHandler::Chatting(PacketData* _packetData)
{
	ChattingPacket* packetData = (ChattingPacket*)_packetData;

	set<shared_session> vecSession = g_SessionManager->GetSessions();

	for (shared_session iter : vecSession)
	{
		SendBuffer* pSendBuffer = PacketCreate::ChattingPacketCreate(packetData->m_chattingContent, ePacketType::SToC_Chatting);

		iter->RegisterSend(pSendBuffer);
	}
}

void PacketHandler::Register(shared_session _session, PacketData* _packetData)
{
	UserRegistPacket* packetData = (UserRegistPacket*)_packetData;

	DBObject* dbOBject = DataBaseManager().GetInstance()->PopDBObject();

	SQLLEN temp1 = 0;
	SQLLEN temp2 = 0;
	SQLLEN temp3 = 0;

	dbOBject->BindParam(packetData->m_UserID, &temp1);
	dbOBject->BindParam(packetData->m_Password, &temp2);
	dbOBject->BindParam(&packetData->Score, &temp3);

	auto query = L"INSERT INTO [GameServer].[dbo].[Players] VALUES (?,?,?)";

	if (!dbOBject->Query(query))
	{
		cout << "Query Fail" << endl;
	}

	SendBuffer* pSendBuffer = PacketResultCreate(ePacketResult::Success, ePacketType::SToC_PacketResult);
	_session->RegisterSend(pSendBuffer);
}

SendBuffer* PacketHandler::PacketResultCreate(ePacketResult _packetResult, ePacketType _ePacketType)
{
	SendBuffer* pSendBuffer = new SendBuffer(sizeof(PacketResult));

	PacketResult* chatting = (PacketResult*)pSendBuffer->GetSendBufferAdress();
	chatting->m_PakcetType = _ePacketType;
	chatting->m_iSize = sizeof(PacketResult);

	chatting->m_iResult = (int)_packetResult;

	pSendBuffer->WsaBufSetting();

	return pSendBuffer;
}
