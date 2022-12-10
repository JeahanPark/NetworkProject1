#pragma once
class Transform;
class InteractionObject
{
public:
	InteractionObject();
	virtual ~InteractionObject();
	void virtual Update() = 0;

public:
	bool GetValidCheck() { return m_bVaildCheck; }

public:
	void InteractionPacketSetting(InteractionPacketData* _packet);

protected:
	bool				m_bVaildCheck;
	int					m_iInteractionIndex;
	Transform*			m_transform = nullptr;
	eInteractionType	m_eType;
};

