#pragma once
class ClientSession : public Session
{
protected:
	virtual void PacketeHandle(PacketData* _Packetdata) override;
	virtual void DeleteSession() override;
public:
	void Chatting();
};

