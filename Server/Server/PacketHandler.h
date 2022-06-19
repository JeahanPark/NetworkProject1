#pragma once
enum class PacketType
{
	NONE = 0,
	// 서버에서 클라로

	SToC_Login,
	// 서버에서 클라로

	// 클라에서 서버로

	CToS_Login,
	// 클라에서 서버로

	// 둘다 똑같은 형태로 씀

	Both_Chatting,

	// 둘다 똑같은 형태로 씀
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
