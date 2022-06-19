#pragma once
enum class PacketType
{
	NONE = 0,
	// �������� Ŭ���

	SToC_Login,
	// �������� Ŭ���

	// Ŭ�󿡼� ������

	CToS_Login,
	// Ŭ�󿡼� ������

	// �Ѵ� �Ȱ��� ���·� ��

	Both_Chatting,

	// �Ѵ� �Ȱ��� ���·� ��
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
	char chattingContent[];

	
};

class PacketHandler
{
public:
	static void PacketHandling(PacketData* _Packetdata);

	static void Chatting(ChattingPacket* _Packetdata);
};
