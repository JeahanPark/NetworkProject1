#include "pch.h"
#include "UserObject.h"
#include "State.h"
#include "UserController.h"
#include "Transform.h"

UserObject::UserObject(s_UserController _uerController, const UserData* _userData)
	:	m_userController(_uerController),
		m_state(new State()),
		m_UserData(*_userData)
{
	m_eType = eInteractionType::User;
}

UserObject::~UserObject()
{
	m_userController = nullptr;
	delete m_state;
}

void UserObject::Update()
{
	// 인게임에서 어떤이유로든 나가졌을경우
	if (!m_userController->GetVaildCheck())
	{
		m_bVaildLife = false;
		return;
	}

	// 유저 입력받을 정보가 있냐???
	if (m_userController->GetNeedMoveCheck())
	{
		const XMFLOAT3& dir = m_userController->GetMoveDir();
		const float& moveSpeed = m_userController->GetMoveSpeed();

		m_transform->SetUserMove(dir, moveSpeed);

		// 다받고 클리어 해준다.
		m_userController->ClearUserCommand();
	}
	
	// transform업데이트
	m_transform->Update();

	
}

void UserObject::SettingInitialInGameDataPacket(InitialInGameData* _packet)
{
	SettingInteractionPacket(_packet);
}
