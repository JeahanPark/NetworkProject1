#include "pch.h"
#include "UserObject.h"
#include "UserController.h"
#include "Collision.h"
#include "AttackFireBall.h"
#include "InGameObject.h"


UserObject::UserObject(s_UserController _uerController, const UserData* _userData)
	:	m_userController(_uerController),
		m_UserData(*_userData)
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

	// 스킬Update
	m_SkillManaging.Update();

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

void UserObject::Init()
{
	m_collision = new Collision(shared_from_this());
	m_SkillManaging.InitData(shared_from_this());
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

void UserObject::RecivedDamage(Collision* _recivedDamageTarget)
{
	m_state->SubtractedHealth(1);

	if (m_state->Die())
	{
		// 지금 때린애가 죽였다.
		s_InteractionObejct target = _recivedDamageTarget->GetOwner();

		if (target->GetInteractionType() == eInteractionType::AttackFireBall)
		{
			AttackFireBall* fireBall = static_cast<AttackFireBall*>(target.get());

			s_InteractionObejct targetOwner = fireBall->GetOwner();

			UserObject* user = static_cast<UserObject*>(targetOwner.get());
			user->AddPoint(1);
		}
	}

	PacketHandler::AllUserNotifyRecivedDamage(m_iInteractionIndex, 1);
}

void UserObject::AddPoint(long _lAddPoint)
{
	s_InGameObject inGameObject = InGameManager::GetInstance()->GetInGameObject(GetUserIndex());

	inGameObject->AddPoint(_lAddPoint);

	PacketHandler::AddPoint(inGameObject);
}
