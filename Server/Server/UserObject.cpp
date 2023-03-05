#include "pch.h"
#include "UserObject.h"
#include "UserController.h"


UserObject::UserObject(s_UserController _uerController, const UserData* _userData)
	:	m_userController(_uerController),
		m_UserData(*_userData)
{
	m_userController->SetDie(false);
	m_eType = eInteractionType::User;
}

UserObject::~UserObject()
{
	m_userController = nullptr;
}

void UserObject::Update()
{
	// 인게임에서 어떤이유로든 나가졌을경우
	if (!m_userController->GetVaildCheck())
	{
		m_bVaildLife = false;
		return;
	}

	// 죽었다.
	if (m_state->Die())
	{
		m_bVaildLife = false;
		m_userController->SetDie(true);
		return;
	}

	// 유저 입력받을 정보가 있냐???
	if (m_userController->GetNeedMoveCheck())
	{
		const XMFLOAT3& dirMove = m_userController->GetMoveDir();
		const XMFLOAT3& rotateY = m_userController->GetRotateY();
		float moveSpeed = m_userController->GetMoveSpeed();


		m_transform->SetUserMove(dirMove, rotateY, moveSpeed);

		// 다받고 클리어 해준다.
		m_userController->ClearUserCommand();
	}
	
	// transform업데이트
	m_transform->Update();

	
}

void UserObject::SettingInitialInGameDataPacket(InitialInGameData* _packet)
{
	wcscpy_s(_packet->m_UserID, USER_ID_LENGTH, m_UserData.GetUserID());
	SettingInteractionPacket(_packet);
}
