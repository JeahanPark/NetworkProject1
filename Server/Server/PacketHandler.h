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