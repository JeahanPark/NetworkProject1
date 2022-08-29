#pragma once

class PacketHandler
{
public:
	static void PacketHandling(s_ServerSession _session, PacketData* _PacketData);

	static void Chatting(PacketData* _packetData);

	static void Register(s_ServerSession _session, PacketData* _packetData);

	static SendBuffer* PacketResultCreate(ePacketResult _packetResult, ePacketType _ePacketType);

	static void Login(s_ServerSession _session, PacketData* _packetData);

	static SendBuffer* LoginResultPacketCreate(s_ServerSession _session, ePacketResult _result);
};
