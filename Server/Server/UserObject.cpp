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

void UserObject::ReciveDamage(int _iDamage)
{
	m_state->SubtractedHealth(_iDamage);

	PacketHandler::AllUserNotifyRecivedDamage(m_iInteractionIndex, _iDamage);
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

void UserObject::RecivedCollision(Collision* _sendtarget)
{
	s_InteractionObejct target = _sendtarget->GetOwner();

	if (target->GetInteractionType() == eInteractionType::AttackFireBall)
	{
		if (m_SkillManaging.UsingSkill(eSkillType::Reflection))
		{
			// ���� ���� ���°� �ݻ� �����̸� ���̾ �ñ�

			// ���̾�� ������� �ݴ��������
			XMFLOAT3 dir = _sendtarget->GetTransform()->GetRotateY();

			dir.x = -dir.x;
			dir.z = -dir.z;

			XMVECTOR norDir = XMLoadFloat3(&dir);
			norDir = XMVector3Normalize(norDir);
			XMStoreFloat3(&dir, norDir);

			// �ݴ��ߴٰ� �پ˷��ش�.
			PacketHandler::ShowReflectionEffect(shared_from_this(), dir);



			s_InteractionObejct interaction = InteractionCreator::CreateFireball(shared_from_this(),
				GetTransform()->GetPos(), dir);

			InteractionManager::GetInstance()->AddInteractionObject(interaction);

			return;
		}

		ReciveDamage(1);

		if (m_state->Die())
		{
			AttackFireBall* fireBall = static_cast<AttackFireBall*>(target.get());

			s_InteractionObejct targetOwner = fireBall->GetOwner();

			UserObject* user = static_cast<UserObject*>(targetOwner.get());
			user->AddPoint(1);
		}
	}
	else if (target->GetInteractionType() == eInteractionType::AttackDummy)
	{
		ReciveDamage(1);
	}
	else if (target->GetInteractionType() == eInteractionType::ReflectionItem)
	{
		// �������� �����.
		m_SkillManaging.AddSkill(eSkillType::Reflection);
	}
}

void UserObject::AddPoint(long _lAddPoint)
{
	s_InGameObject inGameObject = InGameManager::GetInstance()->GetInGameObject(GetUserIndex());

	inGameObject->AddPoint(_lAddPoint);

	PacketHandler::AddPoint(inGameObject);
}
