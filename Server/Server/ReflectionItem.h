#pragma once
#include "InteractionObject.h"

class ReflectionItem : public InteractionObject
{
public :
	ReflectionItem(const XMFLOAT3& _vStartPos);
	~ReflectionItem();

	// InteractionObject��(��) ���� ��ӵ�
	virtual void Update() override;

public:
	virtual void Die();

private:
	float m_fLifeTime;
	const float m_fLifeMaxTime = 10;
};

