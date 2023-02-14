#pragma once
class State
{
public:
	State();
	~State();

public:
	float	GetSpeed() { return m_fSpeed; }
	bool	Die() { return m_fHealth <= 0; }
public:
	void SubtractedHealth(float _fValue);

private:
	float m_fSpeed;
	float m_fHealth;
};

