#pragma once
#include "State.h"
#include "Transform.h"

class Collision;
class InteractionObject : public enable_shared_from_this<InteractionObject>
{
public:
	InteractionObject();
	virtual ~InteractionObject();

	// 이거를 만든이유는 생성자에서 처리를 못하는 애들때문에
	// ex) Collision한테 enable_shared_from_this를 넘겨줘야되는데 생성자가 모두 종료된후 _Wptr세팅되기 때문에
	void virtual Init();
	void virtual Update() = 0;

public:
	bool					GetValidLife() { return m_bVaildLife; }
	eInteractionType		GetInteractionType() { return m_eType; }
	int						GetInteractionIndex() { return m_iInteractionIndex; }
	Collision*				GetCollision() { return m_collision; }
	Transform*				GetTransform() { return m_transform; }

public:
	void					SetValidLife(bool _bVaildLife);
	void					SettingInteractionPacket(InteractionPacketData* _packet);
	void virtual			RecivedDamage();

	//	충돌했다. 충돌을 당한건지 한건지 모름
	void virtual			InteractionCollision();

protected:
	bool				m_bVaildLife;
	int					m_iInteractionIndex;
	Transform*			m_transform = nullptr;
	Collision*			m_collision = nullptr;
	State*				m_state = nullptr;
	eInteractionType	m_eType;
};

