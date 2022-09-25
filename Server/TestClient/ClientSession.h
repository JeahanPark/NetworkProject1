#pragma once
class ClientSession : public Session
{
protected:
	virtual void PacketeHandle(PacketData* _Packetdata) override;
	virtual void DeleteSession() override;

public:
	void LoginSuccess();
	bool DIdLogin();

	void Chatting();
	void Login();
	void ChattingEnter();

private:
	bool m_Login = false;
};

