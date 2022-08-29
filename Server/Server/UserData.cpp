#include "pch.h"
#include "UserData.h"

void UserData::SetUserData(WCHAR* _userID, int _iScore)
{
	wcscpy_s(m_UserID, USER_ID_LENGTH, _userID);
}
