#pragma once
class Transform;
class InteractionObject
{
public:
	InteractionObject();
	virtual ~InteractionObject();

	void virtual Update() = 0;
public:
	void InteractionPacketSetting(InteractionPacketData* _packet);
protected:
	int					m_iInteractionIndex;
	Transform*			m_transform = nullptr;
	eInteractionType	m_eType;
};

