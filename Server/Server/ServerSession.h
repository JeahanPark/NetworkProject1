#pragma once
class ServerSession : public Session
{
public:
	ServerSession();

public:
	void InitSession(HANDLE _iocpHandle, SOCKET _socket) override;
	void SetUserData(const WCHAR* _userID, int _iScore, int _iUserIndex);
	UserData* GetUserData();
	bool IsLogin() { return m_userData != nullptr; }

protected:
	void PacketeHandle(BasePacket* _Packetdata) override;
	void DeleteSession() override;
private:
	UserData* m_userData = nullptr;
};

