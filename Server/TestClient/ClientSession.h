#pragma once
class ClientSession : public Session
{
protected:
	virtual void PacketeHandle(PacketData* _Packetdata) override;

public:
	void Chetting();
};

