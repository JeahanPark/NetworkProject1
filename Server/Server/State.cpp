#include "pch.h"
#include "State.h"

State::State() : m_fHealth(2), m_fSpeed(3)
{
}

State::~State()
{
}

void State::SubtractedHealth(float _fValue)
{
	m_fHealth -= _fValue;
}
