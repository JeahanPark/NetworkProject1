#include "pch.h"
#include "InteractionObject.h"
#include "Transform.h"

InteractionObject::InteractionObject() : 
	m_eType(eInteractionType::None),
	m_transform(new Transform()),
	m_iInteractionIndex{-1}
{
}

InteractionObject::~InteractionObject()
{
	delete m_transform;
}

void InteractionObject::InteractionPacketSetting(InteractionPacketData* _packet)
{
	_packet->m_eType = m_eType;
	_packet->m_iInteractionIndex = m_iInteractionIndex;
	_packet->m_vPos = m_transform->GetPos();
}