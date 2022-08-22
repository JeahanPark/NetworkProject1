#pragma once

class PacketHandler
{
public:
	static void PacketHandling(shared_session _session, PacketData* _PacketData);

	static void Chatting(PacketData* _packetData);

	static void Register(shared_session _session, PacketData* _packetData);

	static SendBuffer* PacketResultCreate(ePacketResult _packetResult, ePacketType _ePacketType);
};
