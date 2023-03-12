#pragma once
#include "State.h"
#include "Transform.h"

class Collision;
class InteractionObject : public enable_shared_from_this<InteractionObject>
{
public:
	InteractionObject();
	virtual ~InteractionObject();

	// �̰Ÿ� ���������� �����ڿ��� ó���� ���ϴ� �ֵ鶧����
	// ex) Collision���� enable_shared_from_this�� �Ѱ���ߵǴµ� �����ڰ� ��� ������� _Wptr���õǱ� ������
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

	//	�浹�ߴ�. �浹�� ���Ѱ��� �Ѱ��� ��
	void virtual			InteractionCollision();

protected:
	bool				m_bVaildLife;
	int					m_iInteractionIndex;
	Transform*			m_transform = nullptr;
	Collision*			m_collision = nullptr;
	State*				m_state = nullptr;
	eInteractionType	m_eType;
};

