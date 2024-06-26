#pragma once
class InteractionCreator
{
public:
	static s_InteractionObejct CreateUserInteraction(s_UserController _userController, const UserData* _userData);
	static s_InteractionObejct CreateDummyInteraction(const XMFLOAT3& _startPos);
	static s_InteractionObejct CreateFireball(s_InteractionObejct _owner, const XMFLOAT3& _vStartPos, const XMFLOAT3& _vRotateY);
	static s_InteractionObejct CreateReflectionItem(const XMFLOAT3& _vStartPos);
};

