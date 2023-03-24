#include "pch.h"
#include "InteractionCreator.h"
#include "UserObject.h"
#include "AttackDummy.h"
#include "AttackFireBall.h"
#include "ReflectionItem.h"

s_InteractionObejct InteractionCreator::CreateUserInteraction(s_UserController _userController, const UserData* _userData)
{
	s_InteractionObejct interaction = make_shared<UserObject>(_userController, _userData);
	interaction->Init();

	return interaction;
}

s_InteractionObejct InteractionCreator::CreateDummyInteraction(const XMFLOAT3& _startPos)
{
	s_InteractionObejct interaction = make_shared<AttackDummy>(_startPos);
	interaction->Init();

	return interaction;
}

s_InteractionObejct InteractionCreator::CreateFireball(s_InteractionObejct _owner, const XMFLOAT3& _vStartPos, const XMFLOAT3& _vRotateY)
{
	s_InteractionObejct interaction = make_shared<AttackFireBall>(_owner, _vStartPos, _vRotateY);
	interaction->Init();

	return interaction;
}

s_InteractionObejct InteractionCreator::CreateReflectionItem(const XMFLOAT3& _vStartPos)
{
	s_InteractionObejct interaction = make_shared<ReflectionItem>(_vStartPos);
	interaction->Init();
	return interaction;
}
