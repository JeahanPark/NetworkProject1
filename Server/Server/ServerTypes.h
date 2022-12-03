#pragma once


using s_ServerSession = shared_ptr<class ServerSession>;
using s_InGameObject = shared_ptr<class InGameObject>;
using s_UserController = shared_ptr<class UserController>;


enum class eInteractionType
{
	None,
	User
};