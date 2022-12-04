#pragma once
class InteractionObject;
class PacketHandler
{
public:
	static void PacketHandling(s_ServerSession _session, PacketData* _packetData);

	static void PacketSignal(s_ServerSession _session, PacketData* _packetData);

	static void Chatting(PacketData* _packetData);

	static void Register(s_ServerSession _session, PacketData* _packetData);

	static SendBuffer* PacketResultCreate(ePacketResult _packetResult, ePacketType _ePacketType, ePacketSignal _eSignal = ePacketSignal::NONE);
	
	static void Login(s_ServerSession _session, PacketData* _packetData);
	
	static SendBuffer* LoginResultPacketCreate(s_ServerSession _session, ePacketResult _result);
	
	static void ChattingRoomProcess(s_ServerSession _session, ePacketSignal _signal);
	
	static void InGameEnterProcess(s_ServerSession _session, ePacketSignal _signal);

	static void MyUserMove(s_ServerSession _session, PacketData* _packetData);

	static void InGameUpdate(const list<InteractionObject*>& _lisInteraction, s_ServerSession _session);
};
