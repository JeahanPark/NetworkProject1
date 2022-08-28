#pragma once
enum class ePacketType
{
	NONE = 0,
	// 서버에서 클라로

	SToC_Login,
	SToC_Chatting,
	SToC_PacketResult,
	// 서버에서 클라로

	// 클라에서 서버로

	CToS_Login,
	CToS_Chatting,
	CToS_UserRegister,
	// 클라에서 서버로

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
