#pragma once
class UserData
{
public:
	UserData(WCHAR* _userID, int _iScore, int _iUserIndex);

public:
	WCHAR* GetUserID()
	{
		return m_UserID;
	}

	int GetScore()
	{
		return m_iScore;
	}

	int GetUserIndex()
	{
		return m_iUserIndex;
	}

private:
	WCHAR m_UserID[USER_ID_LENGTH];
	int m_iScore;
	int m_iUserIndex;
};

