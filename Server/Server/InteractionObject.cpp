#include "pch.h"
#include "InteractionObject.h"
#include "Collision.h"

static int m_gInteractionIndex = 0;

InteractionObject::InteractionObject() : 
	m_eType(eInteractionType::None),
	m_transform(new Transform()),
	m_iInteractionIndex{m_gInteractionIndex++},
	m_bVaildLife(true),
	m_state(new State()),
	m_collision(nullptr)
{
	
}

InteractionObject::~InteractionObject()
{
	delete m_transform;
	delete m_collision;
	delete m_state;
}

void InteractionObject::Init()
{
	m_collision = new Collision(shared_from_this());
}

void InteractionObject::SetValidLife(bool _bVaildLife)
{
	m_bVaildLife = _bVaildLife;
}

void InteractionObject::SettingInteractionPacket(InteractionPacketData* _packet)
{
	_packet->m_eType = m_eType;
	_packet->m_iInteractionIndex = m_iInteractionIndex;
	_packet->m_vPos = m_transform->GetPos();
	_packet->m_fMoveSpeed = m_transform->GetMoveSpeed();
	_packet->m_vMoveDir = m_transform->GetMoveDir();
	_packet->m_vRotateY = m_transform->GetRotateY();
	_packet->m_bValidLife = m_bVaildLife;
}

void InteractionObject::RecivedCollision(Collision* _sendtarget)
{

}

void InteractionObject::SendCollision()
{

}

void InteractionObject::Die()
{
}
