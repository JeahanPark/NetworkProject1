#pragma once
enum class ePacketType
{
	NONE = 0,
	// �������� Ŭ���

	SToC_Login,
	SToC_Chatting,
	SToC_PacketResult,
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
	ePacketType	m_TargetPakcetType;
	int			m_iResult;
};

struct UserRegistPacket : PacketData
{
public:
	WCHAR m_UserID[64];
	WCHAR m_Password[64];
	int Score;
};

struct LoginPacket : PacketData
{
public:
	WCHAR m_UserID[64];
	WCHAR m_Password[64];
};

struct ChattingPacket : PacketData
{
public:
	WCHAR m_chattingContent[300];
};

class PacketCreate
{
public:
	static SendBuffer* ChattingPacketCreate(WCHAR* _content, ePacketType _ePacketType);
};
