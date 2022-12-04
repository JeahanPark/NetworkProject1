#pragma once
class State
{
public:
	State();
	~State();

public:
	const float& GetSpeed() { return m_fSpeed; }

private:
	float m_fSpeed;
};

