#pragma once
class InteractionObject
{
public:
	InteractionObject();
	virtual ~InteractionObject();

protected:
	eInteractionType m_eType;
};

