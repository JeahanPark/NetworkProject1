#pragma once
class ChattingObject
{
public:
	ChattingObject(s_ServerSession _session);
	~ChattingObject();

	bool SameSession(int _iUserIndex);
	bool UseChatting();
	void SendChatting(ChattingPacket* packetData);
private:
	s_ServerSession m_session = nullptr;
};