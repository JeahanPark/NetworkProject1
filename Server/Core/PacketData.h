#pragma once
enum class ePacketType
{
	NONE = 0,
	// �������� Ŭ���

	SToC_Chatting,
	SToC_PacketResult,
	SToC_LoginResult,
	// �������� Ŭ���

	// Ŭ�󿡼� ������

	CToS_Login,
	CToS_Chatting,
	CToS_UserRegister,
	// Ŭ�󿡼� ������

	END,
};

enum class ePacketResult
{
	Success,
	Fail,
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
};

struct UserRegistPacket : PacketData
{
public:
	WCHAR m_UserID[USER_ID_LENGTH] = {};
	WCHAR m_Password[PASSWORD_LENGTH] = {};
	int Score;
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
	int Score;
};

struct ChattingPacket : PacketData
{
public:
	WCHAR m_chattingContent[CHATTING_LENGTH] = {};
};

class PacketCreate
{
public:
	static SendBuffer* ChattingPacketCreate(WCHAR* _content, ePacketType _ePacketType);
};
