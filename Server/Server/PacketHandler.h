#pragma once

class PacketHandler
{
public:
	static void PacketHandling(Session* _session, PacketData* _Packetdata);

	static void Chatting(ChattingPacket* _Packetdata);
};