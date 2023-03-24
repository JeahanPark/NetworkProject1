#pragma once
enum class ePacketType
{
	NONE = 0,
	
	// 결과 보낼때 쓰자
	Result,

	// 데이터는 안보내고 간단하게 신호만 보낼경우 이걸로 쓰자
	Signal,


	// 서버에서 클라로

	SToC_Chatting,
	SToC_PacketResult,
	SToC_LoginResult,
	SToC_InGameUpdate,
	SToC_InitialInGameData,
	SToC_NewUserInteraction,
	SToC_RecivedDamage,
	SToC_UserRiseAgain,
	SToC_UpdatetMySkill,
	SToC_UpdatePoint,
	// 서버에서 클라로

	// 클라에서 서버로

	CToS_Login,
	CToS_Chatting,
	CToS_UserRegister,
	CToS_MyUserMove,
	CToS_UpdatePoint,
	// 클라에서 서버로

	END,
};

enum class ePacketResult
{
	NONE = 0,
	Success,
	Fail,

	// 채팅방 진입 나가기에 대한 결과
	ChattingRoomEnter_Not_Login,
	ChattingRoomEnter_Already_In,
	ChattingRoomExit_Not_Exist,

	// 인게임 진입 나가기에 대한 결과
	InGameEnter_Not_Login,
	InGameEnter_Already_In,
	InGameEnter_InChattingRoom,
	InGameExit_Not_Exist,
};


enum class ePacketSignal
{
	NONE = 0,


	// 채팅방 결과
	Signal_ChattingRoomEnter,
	Signal_ChattingRoomExit,
	Signal_ChattingRoomExit_Not_Exist,

	// 인게임 입장, 나가기 결과
	Signal_InGameEnter,
	Signal_InGameExit,

	// 이거를 받아으면 그뒤로 인게임 패킷을 받기 시작한다.
	Signal_InitialInGameData,

	// 살려달라고 요청
	Signal_InGameUserRiseAgain,

	// 공격
	Signal_InGameAttack,
};
#pragma pack(1)
struct BasePacket
{
public:
	ePacketType	m_PakcetType;
	int			m_iSize;

};
#pragma pack(1)
struct PacketResult : BasePacket
{
public:
	ePacketType			m_TargetPakcetType;
	ePacketResult		m_Result;

	// m_TargetPakcetType이 Signal일경우 쓴다.
	ePacketSignal		m_SignalType = ePacketSignal::NONE;
};
#pragma pack(1)
struct UserRegistPacket : BasePacket
{
public:
	WCHAR m_UserID[USER_ID_LENGTH] = {};
	WCHAR m_Password[PASSWORD_LENGTH] = {};
	int m_iScore;
};
#pragma pack(1)
struct LoginRequestPacket : BasePacket
{
public:
	WCHAR m_UserID[64];
	WCHAR m_Password[64];
};
#pragma pack(1)
struct LoginResultPacket : PacketResult
{
public:
	WCHAR m_UserID[USER_ID_LENGTH] = {};
	int m_iScore;
	int m_iUserIndex;
};
#pragma pack(1)
struct ChattingPacket : BasePacket
{
public:
	WCHAR m_chattingContent[CHATTING_LENGTH] = {};
};
#pragma pack(1)
struct SignalPacket : BasePacket
{
public:
	ePacketSignal m_ePacketSignal;
};

class PacketCreate
{
public:
	static SendBuffer* ChattingPacketCreate(WCHAR* _content, ePacketType _ePacketType);
};