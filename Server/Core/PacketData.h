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

	// 클라에서 서버로

	END,
};

struct PacketData
{
public:
	PacketType	m_PakcetType;
	int			m_iSize;
};

struct LoginPacket : PacketData
{

};

struct ChattingPacket : PacketData
{
public:
	char chattingContent[500];


};

class PacketCreate
{
public:
	static SendBuffer* ChattingPacketCreate(char* _content, PacketType _ePacketType);
};
