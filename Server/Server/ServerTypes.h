#pragma once


using s_ServerSession = shared_ptr<class ServerSession>;
using s_InGameObject = shared_ptr<class InGameObject>;
using s_UserController = shared_ptr<class UserController>;
using s_InteractionObejct = shared_ptr<class InteractionObject>;
using mapInGame = std::map<int, s_InGameObject>;
using lisInteraction = std::list<s_InteractionObejct>;
enum class eInteractionType
{
	None,
	User,
	AttackDummy,
	AttackFireBall,
	Max
};

enum class eCollisionType
{
	None = 0,
	Send = 1,
	Recive = 2,
	Max,
};

enum class eAxisType
{
	Axis_X,
	Axis_Y,
	Axis_Z,
};

// 어케해야될지... 모르겟음
const int maxInteractionCount = 10;

const int noneInteractionIndex = -1;

const XMFLOAT3 RightVector = { 1,0,0 };
const XMFLOAT3 LeftVector = { -1,0,0 };

const XMFLOAT3 UpVector = { 0,1,0 };
const XMFLOAT3 DownVector = { 0,-1,0 };

const XMFLOAT3 FowardVector = { 0,0,1 };
const XMFLOAT3 BackVector = { 0,0,-1 };