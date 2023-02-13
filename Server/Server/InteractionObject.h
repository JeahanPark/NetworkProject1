#pragma once
class Transform;
class Collision;
class InteractionObject : public enable_shared_from_this<InteractionObject>
{
public:
	InteractionObject();
	virtual ~InteractionObject();
	void virtual Update() = 0;

public:
	bool					GetValidLife() { return m_bVaildLife; }
	const eInteractionType& GetInteractionType() { return m_eType; }
	int						GetInteractionIndex() { return m_iInteractionIndex; }
	Collision*				GetCollision() { return m_collision; }
	Transform*		GetTransform() { return m_transform; }
public:
	void					SettingInteractionPacket(InteractionPacketData* _packet);

protected:
	bool				m_bVaildLife;
	int					m_iInteractionIndex;
	Transform*			m_transform = nullptr;
	Collision*			m_collision = nullptr;
	eInteractionType	m_eType;
};

