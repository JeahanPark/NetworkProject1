#pragma once

class PacketHandler
{
public:
	static void PacketHandling(shared_session _session, PacketData* _PacketData);

	static void Chatting(PacketData* _packetData);

	static void Register(PacketData* _packetData);
};
