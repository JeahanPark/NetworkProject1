#pragma once

class ServerSession : public Session
{
public:
	UserData* GetUserData();

protected:
	virtual void PacketeHandle(PacketData* _Packetdata) override;

private:
	UserData m_userData;
};

