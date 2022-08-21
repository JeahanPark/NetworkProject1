#include "pch.h"
#include "PacketHandler.h"

#define SESSION_LOG(SessionNumber, LogName) cout << SessionNumber << " , " << LogName << endl;

void PacketHandler::PacketHandling(shared_session _session, PacketData* _packetData)
{
	switch (_packetData->m_PakcetType)
	{
	case PacketType::CToS_Login:
		break;

	case PacketType::CToS_Chatting:
		Chatting(_packetData);
		SESSION_LOG(_session->GetSessionNumber(), "CToS_Chatting")
		break;
	case PacketType::SToC_Chatting:
		SESSION_LOG(_session->GetSessionNumber(), "SToC_Chatting")
		break;
	case PacketType::CToS_UserRegister:
		Register(_packetData);
		SESSION_LOG(_session->GetSessionNumber(), "CToS_UserRegister")
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
		SendBuffer* pSendBuffer = PacketCreate::ChattingPacketCreate(packetData->m_chattingContent, PacketType::SToC_Chatting);

		iter->RegisterSend(pSendBuffer);
	}
}

void PacketHandler::Register(PacketData* _packetData)
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
}
