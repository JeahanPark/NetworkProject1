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
		const float& moveSpeed = m_userController->GetMoveSpeed();

		m_transform->SetUserMove(dir, moveSpeed);

		// �ٹް� Ŭ���� ���ش�.
		m_userController->ClearUserCommand();
	}
	
	// transform������Ʈ
	m_transform->Update();

	
}
