#include "pch.h"
#include "UserObject.h"
#include "UserController.h"


UserObject::UserObject(s_UserController _uerController, const UserData* _userData)
	:	m_userController(_uerController),
		m_UserData(*_userData),
	m_SkillManaging(this)
{
	m_userController->SetDie(false);
	m_eType = eInteractionType::User;
	m_SkillManaging.InitSkill();
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

	// �׾���.
	if (m_state->Die())
	{
		m_bVaildLife = false;
		m_userController->SetDie(true);
		return;
	}

	// ��ųUpdate
	m_SkillManaging.Update();

	// ���� �Է¹��� ������ �ֳ�???
	if (m_userController->GetNeedMoveCheck())
	{
		const XMFLOAT3& dirMove = m_userController->GetMoveDir();
		const XMFLOAT3& rotateY = m_userController->GetRotateY();
		float moveSpeed = m_userController->GetMoveSpeed();


		m_transform->SetUserMove(dirMove, rotateY, moveSpeed);

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

int UserObject::GetUserIndex()
{
	return m_UserData.GetUserIndex();
}

bool UserObject::UseSkiil()
{
	return m_SkillManaging.UseSkill();
}

eSkillType UserObject::GetCrtSkill()
{
	return m_SkillManaging.GetCrtSkill();
}
