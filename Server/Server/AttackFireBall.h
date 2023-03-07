#pragma once
#include "InteractionObject.h"

class AttackFireBall : public InteractionObject
{
public:
	AttackFireBall(XMFLOAT3 _vStartPos, XMFLOAT3 _vMoveDir);
	~AttackFireBall();

public:
	virtual void Init();
	virtual void Update() override;

private:
	XMFLOAT3 m_vMoveDir;
	float m_fLifeTime;
	const float m_fLifeMaxTime = 4;
};

