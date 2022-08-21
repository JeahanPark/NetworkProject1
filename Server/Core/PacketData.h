#pragma once
enum class PacketType
{
	NONE = 0,
	// 서버에서 클라로

	SToC_Login,
	SToC_Chatting,

	// 서버에서 클라로

	// 클라에서 서버로

	CToS_Login,
	CToS_Chatting,
	CToS_UserRegister,
	// 클라에서 서버로

	END,
};

struct PacketData
{
public:
	PacketType	m_PakcetType;
	int			m_iSize;
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

};

struct ChattingPacket : PacketData
{
public:
	char m_chattingContent[500];


};

class PacketCreate
{
public:
	static SendBuffer* ChattingPacketCreate(char* _content, PacketType _ePacketType);
};
