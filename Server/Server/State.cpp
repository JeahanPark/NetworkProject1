#include "pch.h"
#include "State.h"

State::State()
{
}

State::~State()
{
}

void State::SubtractedHealth(float _fValue)
{
	m_fHealth -= _fValue;
}
