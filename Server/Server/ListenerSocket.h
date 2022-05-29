#pragma once
class ListenerSocket
{
public:
	ListenerSocket();
	~ListenerSocket();

public:
	LPFN_CONNECTEX		m_fnConnectEX = nullptr;
	LPFN_DISCONNECTEX	m_fnDisconnectEX = nullptr;
	LPFN_ACCEPTEX		m_fnAcceptEX = nullptr;
public:
	void Init();
	void SessionAccept(AcceptPaket* _acceptPacket);

private:
	SOCKET						m_listenSocket;
	HANDLE						m_iocpHandle;
};