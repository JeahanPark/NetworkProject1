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

	// Ŭ�󿡼� ������

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

class ClientPacketHandler
{
public:
	static void RecievePacketHandling(char* _buffer);
	static void Recieve_Chatting(ChattingPacket* _chattingPacket);

	// Send
	static char* Send_Chatting();
};