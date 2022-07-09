#pragma once

struct ChattingPacket;
class ClientPacketHandler
{
public:
	static void RecievePacketHandling(PacketData* _Packetdata);
	static void Recieve_Chatting(ChattingPacket* _chattingPacket);

	// Send
	static SendBuffer* Send_Chatting();
};