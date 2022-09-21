#include "pch.h"
#include "UserData.h"

UserData::UserData(WCHAR* _userID, int _iScore, int _iUserIndex) : m_iScore(_iScore), m_iUserIndex(_iUserIndex)
{
	wcscpy_s(m_UserID, USER_ID_LENGTH, _userID);
}
