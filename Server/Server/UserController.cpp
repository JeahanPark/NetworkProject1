#include "pch.h"
#include "UserController.h"

UserController::UserController() : m_vMoveDir{0,0,0}, m_bNeedMoveCheck(false)
{
}

UserController::~UserController()
{
}

void UserController::SetUserMove(MyUserMovePacket* _packetData)
{
	m_vMoveDir = _packetData->m_vDir;


	m_bNeedMoveCheck = true;
}

void UserController::ClearUserCommand()
{
	m_bNeedMoveCheck = false;

	m_vMoveDir = { 0,0,0 };
}
