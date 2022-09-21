#pragma once
class ServerSession : public Session
{
public:
	void SetUserData(WCHAR* _userID, int _iScore, int _iUserIndex);
	UserData* GetUserData();

protected:
	virtual void PacketeHandle(PacketData* _Packetdata) override;
	virtual void DeleteSession() override;
private:
	UserData* m_userData = nullptr;
};

