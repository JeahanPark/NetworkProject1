#pragma once
#include "pch.h"


struct InGameEnterSuccess : PacketData
{
	int						m_iInteractionIndex;
	eInteractionType		m_eType;
};


struct MyUserMovePacket : PacketData
{
	XMFLOAT3	m_vDir;
	float		m_fMoveSpeed;
};

struct InteractionPacketData
{
	int						m_iInteractionIndex;
	eInteractionType		m_eType;
	bool					m_ValidLife;
	XMFLOAT3				m_vPos;
	XMFLOAT3				m_vDir;
	float					m_fMoveSpeed;
};

struct InGameUpdatePacket : PacketData
{
	int						m_iInteractionCount;
	// 이뒤에 바이트 데이터가 있다.
};