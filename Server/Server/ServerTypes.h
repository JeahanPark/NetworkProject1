#pragma once


using s_ServerSession = shared_ptr<class ServerSession>;
using s_InGameObject = shared_ptr<class InGameObject>;
using s_UserController = shared_ptr<class UserController>;

using mapInGame = std::map<int, s_InGameObject>;
enum class eInteractionType
{
	None,
	User
};
// 어케해야될지... 모르겟음
const int maxInteractionCount = 10;