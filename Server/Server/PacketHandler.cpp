#include "pch.h"
#include "PacketHandler.h"
#include "UserData.h"
#include "InGameObject.h"
#include "InteractionObject.h"
#include "UserObject.h"
#include "UserController.h"

#define SESSION_LOG(SessionNumber, LogName) cout << SessionNumber << " , " << LogName << endl;

void PacketHandler::PacketHandling(s_ServerSession _session, BasePacket* _packetData)
{
	switch (_packetData->m_PakcetType)
	{
	case ePacketType::Signal:
		PacketSignal(_session, _packetData);
		break;
#pragma region  클라에서 서버로
	case ePacketType::CToS_Login:
		Login(_session, _packetData);
		//SESSION_LOG(_session->GetSessionNumber(), "CToS_Login")
		break;
	case ePacketType::CToS_Chatting:
		Chatting(_packetData);
		//SESSION_LOG(_session->GetSessionNumber(), "CToS_Chatting")
			break;
	case ePacketType::CToS_UserRegister:
		Register(_session, _packetData);
		//SESSION_LOG(_session->GetSessionNumber(), "CToS_UserRegister")
			break;
	case ePacketType::CToS_MyUserMove:
		MyUserMove(_session, _packetData);
		//SESSION_LOG(_session->GetSessionNumber(), "CToS_MyUserMove")
			break;
#pragma endregion



#pragma region 서버에서 클라로
	case ePacketType::SToC_Chatting:
		//SESSION_LOG(_session->GetSessionNumber(), "SToC_Chatting")
			break;
	case ePacketType::SToC_PacketResult:
		//SESSION_LOG(_session->GetSessionNumber(), "SToC_PacketResult")
			break;
	case ePacketType::SToC_LoginResult:
		//SESSION_LOG(_session->GetSessionNumber(), "SToC_LoginResult")
			break;
	case ePacketType::SToC_InGameUpdate:
		//SESSION_LOG(_session->GetSessionNumber(), "SToC_InGameUpdate")
			break;
	case ePacketType::SToC_InitialInGameData:
		//SESSION_LOG(_session->GetSessionNumber(), "SToC_InitialInGameData")
			break;
	case ePacketType::SToC_NewUserInteraction:
		//ESSION_LOG(_session->GetSessionNumber(), "SToC_NewUserInteraction")
			break;
	case ePacketType::SToC_RecivedDamage:
		//SESSION_LOG(_session->GetSessionNumber(), "SToC_RecivedDamage")
			break;
	case ePacketType::SToC_UserRiseAgain:
		//SESSION_LOG(_session->GetSessionNumber(), "SToC_UserRiseAgain")
			break;
	default:
		break;
#pragma endregion
	}
}

void PacketHandler::PacketSignal(s_ServerSession _session, BasePacket* _packetData)
{
	SignalPacket* packetSignal = (SignalPacket*)_packetData;

	switch (packetSignal->m_ePacketSignal)
	{
	case ePacketSignal::Signal_ChattingRoomEnter:
	case ePacketSignal::Signal_ChattingRoomExit:
		ChattingRoomProcess(_session, packetSignal->m_ePacketSignal);
		SESSION_LOG(_session->GetSessionNumber(), "Chatting," + (int)packetSignal->m_ePacketSignal)
		break;
	case ePacketSignal::Signal_InGameEnter:
	case ePacketSignal::Signal_InGameExit:
		InGameEnterProcess(_session, packetSignal->m_ePacketSignal);
		SESSION_LOG(_session->GetSessionNumber(), "InGame," + (int)packetSignal->m_ePacketSignal)
		break;
	case ePacketSignal::Signal_InitialInGameData:
		InitialInGame(_session);
		SESSION_LOG(_session->GetSessionNumber(), "InitialInGameData," + (int)packetSignal->m_ePacketSignal)
		break;
	case ePacketSignal::Signal_InGameUserRiseAgain:
		UserRiseAgain(_session);
		SESSION_LOG(_session->GetSessionNumber(), "Signal_InGameUserRiseAgain," + (int)packetSignal->m_ePacketSignal)
		break;
	case ePacketSignal::Signal_InGameAttack:
		UserAttack(_session);
		SESSION_LOG(_session->GetSessionNumber(), "Signal_InGameAttack," + (int)packetSignal->m_ePacketSignal)
		break;
	}
}

void PacketHandler::Chatting(BasePacket* _packetData)
{
	ChattingPacket* packetData = (ChattingPacket*)_packetData;

	ChattingManager().GetInstance()->AllSendChatting(packetData);
}

void PacketHandler::Register(s_ServerSession _session, BasePacket* _packetData)
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
		//cout << "Query Fail" << endl;
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


	return pSendBuffer;
}

void PacketHandler::Login(s_ServerSession _session, BasePacket* _packetData)
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
		//cout << "Query Fail" << endl;
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
			int iUserIndex = _session->GetUserData()->GetUserIndex();
			// 인게임 진입
			if (InGameManager().GetInstance()->ExistInGameObject(iUserIndex))
			{
				// 이미 인게임이다.
				pSendBuffer = PacketResultCreate(ePacketResult::InGameEnter_Already_In, ePacketType::Signal, ePacketSignal::Signal_InGameEnter);
			}
			else
			{
				// 인게임 진입성공
				pSendBuffer = PacketResultCreate(ePacketResult::Success, ePacketType::Signal, ePacketSignal::Signal_InGameEnter);
			}
				
		}
		else // 로그인을 안했다.
			pSendBuffer = PacketResultCreate(ePacketResult::InGameEnter_Not_Login, ePacketType::Signal, ePacketSignal::Signal_InGameEnter);
	}
	else
	{
		// 인게임 나가기
		if (InGameManager().GetInstance()->DeleteInGameObject(_session))
			pSendBuffer = PacketResultCreate(ePacketResult::Success, ePacketType::Signal, ePacketSignal::Signal_InGameExit);
		else // 인게임에 이유저가 없음
			pSendBuffer = PacketResultCreate(ePacketResult::InGameExit_Not_Exist, ePacketType::Signal, ePacketSignal::Signal_InGameExit);
	}

	if (pSendBuffer != nullptr)
		_session->RegisterSend(pSendBuffer);
}

void PacketHandler::MyUserMove(s_ServerSession _session, BasePacket* _packetData)
{
	MyUserMovePacket* packetData = (MyUserMovePacket*)_packetData;

	s_InGameObject inGameObject = InGameManager().GetInstance()->GetInGameObject(_session->GetUserData()->GetUserIndex());

	if (inGameObject == nullptr)
	{
		return;
	}

	inGameObject->MyUserMove(packetData);
}

void PacketHandler::InGameUpdate(const list<s_InteractionObejct>& _lisInteraction, s_ServerSession _session)
{
	int interactionCount = (int)_lisInteraction.size();

	if (interactionCount == 0)
		return;

	int interactionPacketSize = sizeof(InteractionPacketData);
	int iInGameUpdatePacketSize = sizeof(InGameUpdatePacket);

	int iTotalInteractionPacket = interactionPacketSize * interactionCount;
	int iTotalPacketSize = iInGameUpdatePacketSize + iTotalInteractionPacket;

	SendBuffer* pSendBuffer = new SendBuffer(iTotalPacketSize);
	
	InGameUpdatePacket* InGameUpdate = (InGameUpdatePacket*)pSendBuffer->GetSendBufferAdress();
	InGameUpdate->m_PakcetType = ePacketType::SToC_InGameUpdate;
	InGameUpdate->m_iSize = iTotalPacketSize;
	InGameUpdate->m_iInteractionCount = interactionCount;

	int iArrInteractionIndex = 0;

	for (s_InteractionObejct interaction : _lisInteraction)
	{
		int startBuffer = iInGameUpdatePacketSize + iArrInteractionIndex * interactionPacketSize;

		InteractionPacketData* obejct = (InteractionPacketData*)pSendBuffer->GetSendBufferAdress(startBuffer);
		interaction->SettingInteractionPacket(obejct);
		++iArrInteractionIndex;
	}

	_session->RegisterSend(pSendBuffer);
}

void PacketHandler::InitialInGame(s_ServerSession _session)
{
	// 나의 interactionObject를 생성한다.
	int iMyUserIndex = _session->GetUserData()->GetUserIndex();

	s_InGameObject myIngameObject = InGameManager::GetInstance()->CreateInGameObject(_session);
	s_InteractionObejct user = InteractionCreator::CreateUserInteraction(myIngameObject->GetUserController(), _session->GetUserData());

	// s_InteractionObejct를 생성하고 바로 리스트에 넣으면 Update에서 보낼수도있어서 안됌

	// 나한테 데이터 전달
	// SToC_InitialInGameData
	lisInteraction interactions;
	InteractionManager::GetInstance()->GetInteractionTypeList(eInteractionType::User, interactions);

	// 여기에 내꺼 넣어주기
	interactions.push_back(user);

	int iDataCount = (int)interactions.size();

	int iInitialInGameDataSize = sizeof(InitialInGameData);
	int iInitialInGameDataPacketSize = sizeof(InitialInGameDataPacket);

	int iTotalData = iInitialInGameDataSize * iDataCount;
	int iTotalPacketSize = iInitialInGameDataPacketSize + iTotalData;

	SendBuffer* pSendBuffer = new SendBuffer(iTotalPacketSize);

	InitialInGameDataPacket* InGameUpdate = (InitialInGameDataPacket*)pSendBuffer->GetSendBufferAdress();
	InGameUpdate->m_PakcetType = ePacketType::SToC_InitialInGameData;
	InGameUpdate->m_iSize = iTotalPacketSize;
	InGameUpdate->m_iUserCount = iDataCount;
	InGameUpdate->m_iMyInteractionIndex = user->GetInteractionIndex();

	int ingameIndex = 0;
	for (auto iter : interactions)
	{
		int startBuffer = iInitialInGameDataPacketSize + ingameIndex * iInitialInGameDataSize;

		InitialInGameData* packet = (InitialInGameData*)pSendBuffer->GetSendBufferAdress(startBuffer);

		UserObject* user = static_cast<UserObject*>(iter.get());
		user->SettingInitialInGameDataPacket(packet);

		++ingameIndex;
	}

	_session->RegisterSend(pSendBuffer);

	InGameManager::GetInstance()->InsertInGameObject(myIngameObject);
	InteractionManager::GetInstance()->AddInteractionObject(user);

	// 두쓰레드가 여기를 동시에 탓을때 위에서 미리받은건 추가가 안되있기 때문에 다시한번 더 받는다.
	list<s_InGameObject> lisInGameObject;
	InGameManager::GetInstance()->GetlistInGame(lisInGameObject);

	// 현재 인게임에 접속중인 유저한테
	// SToC_AddUserInteraction

	for (auto iter : lisInGameObject)
	{
		// 내꺼 제외하고 보내기
		if (!iter->SameSession(iMyUserIndex))
			AddUserInteraction(user, iter->GetSession());
	}

}

void PacketHandler::AddUserInteraction(s_InteractionObejct _newUser, s_ServerSession _session)
{
	SendBuffer* pSendBuffer = new SendBuffer(sizeof(NewUserPacket));

	NewUserPacket* packet = (NewUserPacket*)pSendBuffer->GetSendBufferAdress();
	packet->m_PakcetType = ePacketType::SToC_NewUserInteraction;
	packet->m_iSize = sizeof(NewUserPacket);

	UserObject* user = static_cast<UserObject*>(_newUser.get());
	user->SettingInitialInGameDataPacket(&packet->InitData);

	_session->RegisterSend(pSendBuffer);
}

void PacketHandler::AllUserNotifyRecivedDamage(int _iRecivedDamageIndetractionIndex, int _iDamage)
{
	list<s_InGameObject> lisInGameObject;
	InGameManager::GetInstance()->GetlistInGame(lisInGameObject);

	for (auto iter : lisInGameObject)
	{
		// 내꺼 제외하고 보내기
		if (!iter->SameSession(_iRecivedDamageIndetractionIndex))
			RecivedDamage(iter->GetSession(), _iRecivedDamageIndetractionIndex, _iDamage);
	}
}

void PacketHandler::RecivedDamage(s_ServerSession _session, int _iRecivedDamageIndetractionIndex, int _iDamage)
{
	SendBuffer* pSendBuffer = new SendBuffer(sizeof(RecivedDamagePacket));

	RecivedDamagePacket* InGameUpdate = (RecivedDamagePacket*)pSendBuffer->GetSendBufferAdress();
	InGameUpdate->m_PakcetType = ePacketType::SToC_RecivedDamage;
	InGameUpdate->m_iSize = sizeof(RecivedDamagePacket);

	InGameUpdate->m_iInteractionIndex = _iRecivedDamageIndetractionIndex;
	InGameUpdate->m_fReciveDamage = _iDamage;

	_session->RegisterSend(pSendBuffer);
}

void PacketHandler::UserRiseAgain(s_ServerSession _session)
{
	int iMyUserIndex = _session->GetUserData()->GetUserIndex();

	s_InGameObject myIngameObject = InGameManager::GetInstance()->GetInGameObject(iMyUserIndex);

	bool rise = true;
	int interactionIndex = noneInteractionIndex;
	s_InteractionObejct interaction = nullptr;

	if (!myIngameObject->GetUserController()->GetDie())
	{
		//안죽엇는데 살려달래
		rise = false;
		interactionIndex = noneInteractionIndex;
	}
	else
	{
		interaction = InteractionCreator::CreateUserInteraction(myIngameObject->GetUserController(), _session->GetUserData());
		interactionIndex = interaction->GetInteractionIndex();
	}

	SendBuffer* pSendBuffer = new SendBuffer(sizeof(UserRiseAgainPacket));

	UserRiseAgainPacket* packet = (UserRiseAgainPacket*)pSendBuffer->GetSendBufferAdress();
	packet->m_PakcetType = ePacketType::SToC_UserRiseAgain;
	packet->m_iSize = sizeof(UserRiseAgainPacket);

	packet->m_iInteractionIndex = interactionIndex;
	packet->m_bRiseAgain = rise;
	
	UserObject* user = static_cast<UserObject*>(interaction.get());
	user->SettingInitialInGameDataPacket(&packet->InitData);

	_session->RegisterSend(pSendBuffer);

	// 안살려주는거면 알릴필요 없으니 끝
	if (!rise)
		return;

	InteractionManager::GetInstance()->AddInteractionObject(interaction);

	list<s_InGameObject> lisInGameObject;
	InGameManager::GetInstance()->GetlistInGame(lisInGameObject);

	for (auto iter : lisInGameObject)
	{
		// 내꺼 제외하고 보내기
		if (!iter->SameSession(iMyUserIndex))
			AddUserInteraction(interaction, iter->GetSession());
	}
}

void PacketHandler::UserAttack(s_ServerSession _session)
{
	UserObject* user = InteractionManager::GetInstance()->GetUser(_session->GetUserData()->GetUserIndex());

	if (user == nullptr)
		return;

	user->UseSkiil();

	SendBuffer* pSendBuffer = new SendBuffer(sizeof(UpdatetMySkillPacket));

	UpdatetMySkillPacket* packet = (UpdatetMySkillPacket*)pSendBuffer->GetSendBufferAdress();
	packet->m_PakcetType = ePacketType::SToC_UpdatetMySkill;
	packet->m_iSize = sizeof(UpdatetMySkillPacket);

	packet->m_eSkillType = user->GetCrtSkill();

	_session->RegisterSend(pSendBuffer);
}
