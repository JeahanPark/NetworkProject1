#pragma once
#include "InteractionObject.h"

class AttackFireBall : public InteractionObject
{
public:
	AttackFireBall(const XMFLOAT3& _vStartPos, const XMFLOAT3& _vRotateY);
	~AttackFireBall();

public:
	virtual void Init();
	virtual void Update() override;

private:
	float m_fLifeTime;
	const float m_fLifeMaxTime = 4;
};

