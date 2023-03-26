#pragma once
#include "InteractionObject.h"

class AttackFireBall : public InteractionObject
{
public:
	AttackFireBall(s_InteractionObejct _owner, const XMFLOAT3& _vStartPos, const XMFLOAT3& _vRotateY);
	~AttackFireBall();

public:
	virtual void Init();
	virtual void Update() override;
	virtual void SendCollision();

public:
	s_InteractionObejct GetOwner() { return m_Owner; }

private:
	s_InteractionObejct m_Owner;
	float m_fLifeTime;
	const float m_fLifeMaxTime = 4;
};

