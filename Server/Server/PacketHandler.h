#pragma once

class PacketHandler
{
public:
	static void PacketHandling(shared_session _session, PacketData* _Packetdata);

	static void Chatting(ChattingPacket* _Packetdata);
};
