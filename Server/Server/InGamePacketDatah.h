#pragma once
#include "pch.h"


struct MyUserMovePacket : PacketData
{
	XMFLOAT3 m_vDir;
};

struct InteractionPacketData
{
	int						m_iInteractionIndex;
	eInteractionType		m_eType;

	XMFLOAT3				m_vPos;
};

struct InGameUpdatePacket : PacketData
{
	int						m_iInteractionCount;
	InteractionPacketData	m_arrInteraction[maxInteractionCount];
};