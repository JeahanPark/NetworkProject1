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
	SToC_PositionUpdate,
	// �������� Ŭ���

	// Ŭ�󿡼� ������

	CToS_Login,
	CToS_Chatting,
	CToS_UserRegister,
	CToS_MyUserMove,
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
};

struct PacketData
{
public:
	ePacketType	m_PakcetType;
	int			m_iSize;

};

struct PacketResult : PacketData
{
public:
	ePacketType			m_TargetPakcetType;
	ePacketResult		m_Result;

	// m_TargetPakcetType�� Signal�ϰ�� ����.
	ePacketSignal		m_SignalType = ePacketSignal::NONE;
};

struct UserRegistPacket : PacketData
{
public:
	WCHAR m_UserID[USER_ID_LENGTH] = {};
	WCHAR m_Password[PASSWORD_LENGTH] = {};
	int m_iScore;
};

struct LoginRequestPacket : PacketData
{
public:
	WCHAR m_UserID[64];
	WCHAR m_Password[64];
};

struct LoginResultPacket : PacketResult
{
public:
	WCHAR m_UserID[USER_ID_LENGTH] = {};
	int m_iScore;
	int m_iUserIndex;
};

struct ChattingPacket : PacketData
{
public:
	WCHAR m_chattingContent[CHATTING_LENGTH] = {};
};

struct SignalPacket : PacketData
{
public:
	ePacketSignal m_ePacketSignal;
};

struct MyUserMovePacket : PacketData
{
public:
	float m_fDirX;
	float m_fDirY;
};
class PacketCreate
{
public:
	static SendBuffer* ChattingPacketCreate(WCHAR* _content, ePacketType _ePacketType);
};
