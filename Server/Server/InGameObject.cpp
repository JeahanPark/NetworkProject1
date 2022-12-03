#include "pch.h"
#include "InGameObject.h"
#include "UserController.h"
InGameObject::InGameObject(s_ServerSession _session, s_UserController _userController)
	: m_session(_session), m_UserController(_userController)
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

void InGameObject::MyUserMove(MyUserMovePacket* _packetData)
{
	m_UserController->SetUserMove(_packetData);
}
