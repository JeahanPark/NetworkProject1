#include "pch.h"
#include "UserObject.h"
#include "UserController.h"


UserObject::UserObject(s_UserController _uerController, const UserData* _userData)
	:	m_userController(_uerController),
		m_UserData(*_userData)
{
	m_eType = eInteractionType::User;
}

UserObject::~UserObject()
{
	m_userController = nullptr;
}

void UserObject::Update()
{
	// �ΰ��ӿ��� ������ε� �����������
	if (!m_userController->GetVaildCheck())
	{
		m_bVaildLife = false;
		return;
	}

	// ���� �Է¹��� ������ �ֳ�???
	if (m_userController->GetNeedMoveCheck())
	{
		const XMFLOAT3& dir = m_userController->GetMoveDir();
		float moveSpeed = m_userController->GetMoveSpeed();

		m_transform->SetUserMove(dir, moveSpeed);

		// �ٹް� Ŭ���� ���ش�.
		m_userController->ClearUserCommand();
	}
	
	// transform������Ʈ
	m_transform->Update();

	
}

void UserObject::SettingInitialInGameDataPacket(InitialInGameData* _packet)
{
	wcscpy_s(_packet->m_UserID, USER_ID_LENGTH, m_UserData.GetUserID());
	SettingInteractionPacket(_packet);
}
