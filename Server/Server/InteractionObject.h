#pragma once
#include "State.h"
#include "Transform.h"

class Collision;
class InteractionObject : public enable_shared_from_this<InteractionObject>
{
public:
	InteractionObject();
	virtual ~InteractionObject();

	void Init();
	void virtual Update() = 0;

public:
	bool					GetValidLife() { return m_bVaildLife; }
	eInteractionType		GetInteractionType() { return m_eType; }
	int						GetInteractionIndex() { return m_iInteractionIndex; }
	Collision*				GetCollision() { return m_collision; }
	Transform*				GetTransform() { return m_transform; }
public:
	void					SettingInteractionPacket(InteractionPacketData* _packet);
	void virtual			RecivedDamage();
protected:
	bool				m_bVaildLife;
	int					m_iInteractionIndex;
	Transform*			m_transform = nullptr;
	Collision*			m_collision = nullptr;
	State*				m_state = nullptr;
	eInteractionType	m_eType;
};

