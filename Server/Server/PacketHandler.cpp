#include "pch.h"
#include "PacketHandler.h"
#include "UserData.h"

#define SESSION_LOG(SessionNumber, LogName) cout << SessionNumber << " , " << LogName << endl;

void PacketHandler::PacketHandling(s_ServerSession _session, PacketData* _packetData)
{
	switch (_packetData->m_PakcetType)
	{
	case ePacketType::Signal:
		PacketSignal(_session, _packetData);
		break;
#pragma region  클라에서 서버로
	case ePacketType::CToS_Login:
		Login(_session, _packetData);
		SESSION_LOG(_session->GetSessionNumber(), "CToS_Login")
		break;
	case ePacketType::CToS_Chatting:
		Chatting(_packetData);
		SESSION_LOG(_session->GetSessionNumber(), "CToS_Chatting")
			break;
	case ePacketType::CToS_UserRegister:
		Register(_session, _packetData);
		SESSION_LOG(_session->GetSessionNumber(), "CToS_UserRegister")
			break;
#pragma endregion



#pragma region 서버에서 클라로
	case ePacketType::SToC_Chatting:
		SESSION_LOG(_session->GetSessionNumber(), "SToC_Chatting")
			break;
	case ePacketType::SToC_PacketResult:
		SESSION_LOG(_session->GetSessionNumber(), "SToC_PacketResult")
			break;
	case ePacketType::SToC_LoginResult:
		SESSION_LOG(_session->GetSessionNumber(), "SToC_LoginResult")
			break;
	default:
		break;
#pragma endregion
	}
}

void PacketHandler::PacketSignal(s_ServerSession _session, PacketData* _packetData)
{
	SignalPacket* packetSignal = (SignalPacket*)_packetData;

	switch (packetSignal->m_ePacketSignal)
	{
	case ePacketSignal::Signal_ChattingRoomEnter:
	case ePacketSignal::Signal_ChattingRoomExit:
		ChattingRoomProcess(_session, packetSignal->m_ePacketSignal);
	case ePacketSignal::Signal_InGameEnter:
	case ePacketSignal::Signal_InGameExit:
		InGameEnterProcess(_session, packetSignal->m_ePacketSignal);
		break;
	}
}

void PacketHandler::Chatting(PacketData* _packetData)
{
	ChattingPacket* packetData = (ChattingPacket*)_packetData;

	ChattingManager().GetInstance()->AllSendChatting(packetData);
}

void PacketHandler::Register(s_ServerSession _session, PacketData* _packetData)
{
	UserRegistPacket* packetData = (UserRegistPacket*)_packetData;

	DBObject* dbObject = DataBaseManager().GetInstance()->PopDBObject();

	SQLLEN temp1 = 0;
	SQLLEN temp2 = 0;
	SQLLEN temp3 = 0;

	dbObject->BindParam(packetData->m_UserID, &temp1);
	dbObject->BindParam(packetData->m_Password, &temp2);
	dbObject->BindParam(&packetData->m_iScore, &temp3);

	auto query = L"INSERT INTO [GameServer].[dbo].[Players] VALUES (?,?,?)";

	if (!dbObject->Query(query))
	{
		cout << "Query Fail" << endl;
	}

	DataBaseManager().GetInstance()->PushDBObject(dbObject);

	SendBuffer* pSendBuffer = PacketResultCreate(ePacketResult::Success, ePacketType::CToS_UserRegister);
	_session->RegisterSend(pSendBuffer);
}

SendBuffer* PacketHandler::PacketResultCreate(ePacketResult _packetResult, ePacketType _eTargetPacketType, ePacketSignal _eSignal)
{
	SendBuffer* pSendBuffer = new SendBuffer(sizeof(PacketResult));

	PacketResult* chatting = (PacketResult*)pSendBuffer->GetSendBufferAdress();
	chatting->m_PakcetType = ePacketType::SToC_PacketResult;
	chatting->m_iSize = sizeof(PacketResult);

	chatting->m_Result = _packetResult;
	chatting->m_TargetPakcetType = _eTargetPacketType;
	chatting->m_SignalType = _eSignal;

	pSendBuffer->WsaBufSetting();

	return pSendBuffer;
}

void PacketHandler::Login(s_ServerSession _session, PacketData* _packetData)
{
	LoginRequestPacket* packetData = (LoginRequestPacket*)_packetData;

	DBObject* dbObject = DataBaseManager().GetInstance()->PopDBObject();

	WCHAR UserID[USER_ID_LENGTH];
	SQLLEN outUserID = 0;
	dbObject->BindCol(1, SQL_C_WCHAR, USER_ID_LENGTH, &UserID, &outUserID);

	WCHAR Password[PASSWORD_LENGTH] = L"\n";
	SQLLEN outPassword = 0;
	dbObject->BindCol(2, SQL_C_WCHAR, PASSWORD_LENGTH, &Password, &outPassword);

	int iScore;
	SQLLEN outScoreLen = 0;
	dbObject->BindCol(3, SQL_C_LONG, SQL_INTEGER, &iScore, &outScoreLen);

	int iUserIndex;
	SQLLEN outUserIndexLen = 0;
	dbObject->BindCol(4, SQL_C_LONG, SQL_INTEGER, &iUserIndex, &outUserIndexLen);

	SQLLEN temp1 = 0;
	dbObject->BindParam(packetData->m_UserID, &temp1);

	auto query = L"SELECT * FROM [GameServer].[dbo].[Players] WHERE UserID = (?)";

	if (!dbObject->Query(query))
	{
		cout << "Query Fail" << endl;
	}

	ePacketResult packetResult = ePacketResult::Fail;

	if (dbObject->IsValidData())
	{
		int result = wcsncmp(Password,  packetData->m_Password, PASSWORD_LENGTH);
		if (0 == result)
		{
			// 비밀번호가 같다.
			_session->SetUserData(UserID, iScore, iUserIndex);
			packetResult = ePacketResult::Success;
		}
	}

	DataBaseManager().GetInstance()->PushDBObject(dbObject);

	SendBuffer* pSendBuffer = LoginResultPacketCreate(_session, packetResult);
	_session->RegisterSend(pSendBuffer);
}

SendBuffer* PacketHandler::LoginResultPacketCreate(s_ServerSession _session, ePacketResult _result)
{
	SendBuffer* pSendBuffer = new SendBuffer(sizeof(LoginResultPacket));

	LoginResultPacket* LoginResult = (LoginResultPacket*)pSendBuffer->GetSendBufferAdress();
	LoginResult->m_PakcetType = ePacketType::SToC_LoginResult;
	LoginResult->m_iSize = sizeof(LoginResultPacket);

	LoginResult->m_Result = _result;
	LoginResult->m_TargetPakcetType = ePacketType::CToS_Login;
	
	if (_result == ePacketResult::Success)
	{
		wcscpy_s(LoginResult->m_UserID, USER_ID_LENGTH, _session->GetUserData()->GetUserID());
		LoginResult->m_iScore = _session->GetUserData()->GetScore();
	}


	pSendBuffer->WsaBufSetting();

	return pSendBuffer;
}

void PacketHandler::ChattingRoomProcess(s_ServerSession _session, ePacketSignal _signal)
{
	SendBuffer* pSendBuffer = nullptr;
	if (_signal == ePacketSignal::Signal_ChattingRoomEnter)
	{
		if (_session->IsLogin())
		{
			// 룸진입
			if (ChattingManager().GetInstance()->InsertChattingObject(_session))
				pSendBuffer = PacketResultCreate(ePacketResult::Success, ePacketType::Signal, ePacketSignal::Signal_ChattingRoomEnter);
			else // 이미 방에 들어있다.
				pSendBuffer = PacketResultCreate(ePacketResult::ChattingRoomEnter_Already_In, ePacketType::Signal, ePacketSignal::Signal_ChattingRoomEnter);
		}
		else // 로그인을 안했다.
			pSendBuffer = PacketResultCreate(ePacketResult::ChattingRoomEnter_Not_Login, ePacketType::Signal, ePacketSignal::Signal_ChattingRoomEnter);
	}
	else
	{
		// 룸 나가기
		if (ChattingManager().GetInstance()->DeleteChattingObject(_session))
			pSendBuffer = PacketResultCreate(ePacketResult::Success, ePacketType::Signal, ePacketSignal::Signal_ChattingRoomExit);
		else // 채팅방에 이유저가 없음
			pSendBuffer = PacketResultCreate(ePacketResult::ChattingRoomExit_Not_Exist, ePacketType::Signal, ePacketSignal::Signal_ChattingRoomExit);
	}

	if (pSendBuffer != nullptr)
		_session->RegisterSend(pSendBuffer);
}

void PacketHandler::InGameEnterProcess(s_ServerSession _session, ePacketSignal _signal)
{
	SendBuffer* pSendBuffer = nullptr;

	if (ChattingManager().GetInstance()->IsExistentChattingObject(_session))
	{
		// 채팅방에 있다? 정상적인 경우에는 못들어오는데
		pSendBuffer = PacketResultCreate(ePacketResult::InGameEnter_InChattingRoom, ePacketType::Signal, ePacketSignal::Signal_InGameEnter);
	}
	else if (_signal == ePacketSignal::Signal_InGameEnter)
	{
		if (_session->IsLogin())
		{
			// 룸진입
			if (InGameManager().GetInstance()->InsertChattingObject(_session))
				pSendBuffer = PacketResultCreate(ePacketResult::Success, ePacketType::Signal, ePacketSignal::Signal_InGameEnter);
			else // 이미 방에 들어있다.
				pSendBuffer = PacketResultCreate(ePacketResult::InGameEnter_Already_In, ePacketType::Signal, ePacketSignal::Signal_InGameEnter);
		}
		else // 로그인을 안했다.
			pSendBuffer = PacketResultCreate(ePacketResult::InGameEnter_Not_Login, ePacketType::Signal, ePacketSignal::Signal_InGameEnter);
	}
	else
	{
		// 룸 나가기
		if (InGameManager().GetInstance()->DeleteChattingObject(_session))
			pSendBuffer = PacketResultCreate(ePacketResult::Success, ePacketType::Signal, ePacketSignal::Signal_ChattingRoomExit);
		else // 채팅방에 이유저가 없음
			pSendBuffer = PacketResultCreate(ePacketResult::ChattingRoomExit_Not_Exist, ePacketType::Signal, ePacketSignal::Signal_ChattingRoomExit);
	}

	if (pSendBuffer != nullptr)
		_session->RegisterSend(pSendBuffer);
}
