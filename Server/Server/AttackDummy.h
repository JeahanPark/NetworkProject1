#pragma once
#include "InteractionObject.h"

class AttackDummy : public InteractionObject
{
public:
	AttackDummy(const XMFLOAT3& _vStartPos);
	~AttackDummy();

public:
	virtual void Update() override;

private:
	bool m_bMoveUp;
};

