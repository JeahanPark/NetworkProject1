#pragma once


using s_ServerSession = shared_ptr<class ServerSession>;
using s_InGameObject = shared_ptr<class InGameObject>;
using s_UserController = shared_ptr<class UserController>;
using s_InteractionObejct = shared_ptr<class InteractionObject>;
using mapInGame = std::map<int, s_InGameObject>;
enum class eInteractionType
{
	None,
	User,
	AttackDummy
};

enum class eCollisionType
{
	None = 0,
	Send = 1,
	Recive = 2
};

// �����ؾߵ���... �𸣰���
const int maxInteractionCount = 10;