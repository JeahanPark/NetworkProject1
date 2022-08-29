#pragma once
class UserData
{
public:
	void SetUserData(WCHAR* _userID, int _iScore);

	WCHAR* GetUserID()
	{
		return m_UserID;
	}

	int GetScore()
	{
		return m_iScore;
	}

private:
	WCHAR m_UserID[USER_ID_LENGTH];
	int m_iScore;
};

