#pragma once
class Transform;
class InteractionObject
{
public:
	InteractionObject();
	virtual ~InteractionObject();
	void virtual Update() = 0;

public:
	bool					GetValidLife() { return m_bVaildLife; }
	const eInteractionType& GetInteractionType() { return m_eType; }
	int						GetInteractionIndex() { return m_iInteractionIndex; }
public:
	void SettingInteractionPacket(InteractionPacketData* _packet);
protected:
	bool				m_bVaildLife;
	int					m_iInteractionIndex;
	Transform*			m_transform = nullptr;
	eInteractionType	m_eType;
};

