#include "pch.h"
#include "InGameObject.h"

InGameObject::InGameObject(s_ServerSession _session)
{
}

InGameObject::~InGameObject()
{
	m_session = nullptr;
}

bool InGameObject::SameSession(int _iUserIndex)
{
	UserData* userData = m_session->GetUserData();
	if (userData == nullptr)
		return false;

	return userData->GetUserIndex() == _iUserIndex;
}
