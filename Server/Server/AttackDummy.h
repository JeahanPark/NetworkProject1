#pragma once
#include "InteractionObject.h"

class AttackDummy : public InteractionObject
{
public:
	AttackDummy(XMFLOAT3 _vStartPos);
	~AttackDummy();

public:
	virtual void Update() override;

};

