#pragma once
class Session;

enum PacketType
{
	Connect,
	Accept,
};

class Packet : public OVERLAPPED
{

//public:
//	PacketType GetPacketType() { return m_ePacketType; }
//
//protected:
//	PacketType m_ePacketType;
};

class AcceptPaket : public Packet
{
public:
	void		SetSession(Session* session) { m_pSession = session; }
	Session		GetSession() { return *m_pSession; }

private:
	Session* m_pSession = nullptr;
};

