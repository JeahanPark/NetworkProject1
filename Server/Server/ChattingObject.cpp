#include "pch.h"
#include "ChattingObject.h"

ChattingObject::ChattingObject(s_ServerSession _session) : m_session(_session)
{
}

ChattingObject::~ChattingObject()
{
	m_session = nullptr;
}

bool ChattingObject::SameSession(int _iUserIndex)
{
	//m_session->GetUserData()
	return false;
}
