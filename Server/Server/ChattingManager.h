#pragma once
class ChattingObject;
class ChattingManager : public BaseManager<ChattingManager>
{
private:
	 const int m_iMaxChattingUser = 30;
public:
	~ChattingManager();

public:
	bool InsertChattingObject(s_ServerSession _session);
	bool DeleteChattingObject(s_ServerSession _session);
	bool AllSendChatting(ChattingPacket* packetData);

private:
	list<ChattingObject*>	m_lisChatting;
	mutex					m_lockChatting;
};

