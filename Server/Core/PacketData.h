#pragma once
enum class ePacketType
{
	NONE = 0,
	
	// ��� ������ ����
	Result,

	// �����ʹ� �Ⱥ����� �����ϰ� ��ȣ�� ������� �̰ɷ� ����
	Signal,


	// �������� Ŭ���

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
	// �������� Ŭ���

	// Ŭ�󿡼� ������

	CToS_Login,
	CToS_Chatting,
	CToS_UserRegister,
	CToS_MyUserMove,
	CToS_UpdatePoint,
	// Ŭ�󿡼� ������

	END,
};

enum class ePacketResult
{
	NONE = 0,
	Success,
	Fail,

	// ä�ù� ���� �����⿡ ���� ���
	ChattingRoomEnter_Not_Login,
	ChattingRoomEnter_Already_In,
	ChattingRoomExit_Not_Exist,

	// �ΰ��� ���� �����⿡ ���� ���
	InGameEnter_Not_Login,
	InGameEnter_Already_In,
	InGameEnter_InChattingRoom,
	InGameExit_Not_Exist,
};


enum class ePacketSignal
{
	NONE = 0,


	// ä�ù� ���
	Signal_ChattingRoomEnter,
	Signal_ChattingRoomExit,
	Signal_ChattingRoomExit_Not_Exist,

	// �ΰ��� ����, ������ ���
	Signal_InGameEnter,
	Signal_InGameExit,

	// �̰Ÿ� �޾����� �׵ڷ� �ΰ��� ��Ŷ�� �ޱ� �����Ѵ�.
	Signal_InitialInGameData,

	// ����޶�� ��û
	Signal_InGameUserRiseAgain,

	// ����
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

	// m_TargetPakcetType�� Signal�ϰ�� ����.
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