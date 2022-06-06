#pragma once
class PacketHandler
{

public:
	PacketHandler();

	void RegisterSession(Session _session);

private:
	HANDLE						m_iocpHandle;
};

