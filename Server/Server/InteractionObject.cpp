#include "pch.h"
#include "InteractionObject.h"
#include "Transform.h"

InteractionObject::InteractionObject() : 
	m_eType(eInteractionType::None),
	m_transform(new Transform()),
	m_iInteractionIndex{-1},
	m_bVaildLife(true)
{
}

InteractionObject::~InteractionObject()
{
	delete m_transform;
}

void InteractionObject::SettingInteractionPacket(InteractionPacketData* _packet)
{
	_packet->m_eType = m_eType;
	_packet->m_iInteractionIndex = m_iInteractionIndex;
	_packet->m_vPos = m_transform->GetPos();
	_packet->m_fMoveSpeed = m_transform->GetMoveSpeed();
	_packet->m_vDir = m_transform->GetYAxisDir();
	_packet->m_bValidLife = m_bVaildLife;
}
