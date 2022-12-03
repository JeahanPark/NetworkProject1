#include "pch.h"
#include "UserController.h"

UserController::UserController() : m_fDirX(0), m_fDirY(0)
{
}

UserController::~UserController()
{
}

void UserController::SetUserMove(MyUserMovePacket* _packetData)
{
	m_fDirX = _packetData->m_fDirX;
	m_fDirY = _packetData->m_fDirY;
}

void UserController::ClearUserCommand()
{
	m_fDirX = 0;
	m_fDirY = 0;
}
