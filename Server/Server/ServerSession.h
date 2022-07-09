#pragma once
class ServerSession : public Session
{
	virtual void PacketeHandle(PacketData* _Packetdata) override;
};

