#pragma once
class ClientPacketHandler
{
public:
	
	static void RecievePacketHandling(s_ClientSession _session, BasePacket* _packetData);
	static void RecieveLogin(s_ClientSession _session);
	static void Recieve_Chatting(ChattingPacket* _chattingPacket);
	static void PacketSignal(s_ClientSession _session, BasePacket* _packetData);
	
	// Send
	static SendBuffer* Send_Chatting();
	static SendBuffer* Send_Login();
	static SendBuffer* Send_ChattingEnter();
};