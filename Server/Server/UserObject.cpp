#include "pch.h"
#include "UserObject.h"
#include "State.h"
#include "UserController.h"
#include "Transform.h"

UserObject::UserObject(s_UserController _uerController, int _iUserIndex)
	:	m_userController(_uerController),
		m_state(new State())
{
	m_eType = eInteractionType::User;
	m_iInteractionIndex = _iUserIndex;
}

UserObject::~UserObject()
{
	m_userController = nullptr;
	delete m_state;
}

void UserObject::Update()
{
	// 이동해야되나?
	if (m_userController->GetNeedMoveCheck())
	{
		const XMFLOAT3& dir = m_userController->GetMoveDir();
		float speed = m_state->GetSpeed();

		m_transform->MovePos(dir, speed);
	}


	m_userController->ClearUserCommand();
}
