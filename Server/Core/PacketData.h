#pragma once
enum class ePacketType
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

struct PacketResult
{
public:
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

};

struct ChattingPacket : PacketData
{
public:
	char m_chattingContent[500];


};

class PacketCreate
{
public:
	static SendBuffer* ChattingPacketCreate(char* _content, ePacketType _ePacketType);
};
