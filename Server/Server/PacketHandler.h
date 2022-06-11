#pragma once
class PacketHandler
{

public:
	PacketHandler();
private:
};

enum PacketType
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

	int GetHeaderSize()
	{
		return sizeof(PacketData);
	}

	virtual int GetPacketSize()
	{
		return GetHeaderSize();
	}
};

struct CToS_Login : PacketData
{

};

struct CToS_Chatting : PacketData
{
	char chattingContent[];
};