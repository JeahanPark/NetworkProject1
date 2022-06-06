#pragma once


class ListenerSession : public Session
{
public:
	ListenerSession();
	~ListenerSession();

public:
	LPFN_ACCEPTEX		m_fnAcceptEX = nullptr;
public:
	void Init();
	void RegisterAccept(AcceptPaket* _acceptPacket);
};