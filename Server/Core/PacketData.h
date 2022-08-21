#pragma once
enum class PacketType
{
	NONE = 0,
	// �������� Ŭ���

	SToC_Login,
	SToC_Chatting,

	// �������� Ŭ���

	// Ŭ�󿡼� ������

	CToS_Login,
	CToS_Chatting,
	CToS_UserRegister,
	// Ŭ�󿡼� ������

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
