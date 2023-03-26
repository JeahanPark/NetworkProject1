#pragma once
#include "InteractionObject.h"

class ReflectionItem : public InteractionObject
{
public :
	ReflectionItem(const XMFLOAT3& _vStartPos);
	~ReflectionItem();

	// InteractionObject을(를) 통해 상속됨
	virtual void Update() override;

	virtual void SendCollision();

public:
	virtual void Die();

private:
	float m_fLifeTime;
	const float m_fLifeMaxTime = 10;
};

