#pragma once
class ChattingObject;
class ChattingManager : public BaseManager<ChattingManager>
{
public:
	~ChattingManager();

public:
	void InsertChattingObject(s_ServerSession _session);
	void DeleteChattingObject(s_ServerSession _session);
	set<ChattingObject*> GetChattingObjects();
private:
	set<ChattingObject*>	m_setChatting;
	mutex					m_lockChatting;
};

