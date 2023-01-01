#include "pch.h"
#include "UserController.h"

UserController::UserController() : m_vMoveDir{0,0,0}, m_bNeedMoveCheck(false), m_bValidCheck(true), m_fMoveSpeed(0)
{
}

UserController::~UserController()
{
}

void UserController::SetUserMove(MyUserMovePacket* _packetData)
{
	m_vMoveDir = _packetData->m_vDir;
	m_fMoveSpeed = _packetData->m_fMoveSpeed;

	m_bNeedMoveCheck = true;
}

void UserController::SetVaildCheck(bool _bVaild)
{
	m_bValidCheck = _bVaild;
}

void UserController::ClearUserCommand()
{
	m_bNeedMoveCheck = false;

	m_vMoveDir = { 0,0,0 };
	m_fMoveSpeed = 0;
}
