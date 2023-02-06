#pragma once
#include "pch.h"



struct MyUserMovePacket : PacketData
{
	XMFLOAT3	m_vDir;
	float		m_fMoveSpeed;
};

struct InteractionPacketData
{
	int						m_iInteractionIndex;
	eInteractionType		m_eType;
	bool					m_bValidLife;
	XMFLOAT3				m_vPos;
	XMFLOAT3				m_vDir;
	float					m_fMoveSpeed;
};

struct InGameUpdatePacket : PacketData
{
	int						m_iInteractionCount;
	// 이뒤에 바이트 데이터 InteractionPacketData 가 있다.
};

struct InitialInGameData : InteractionPacketData
{
	WCHAR					m_UserID[USER_ID_LENGTH] = {};
};

struct InitialInGameDataPacket : PacketData
{
	int						m_iMyInteractionIndex;
	int						m_iUserCount;
	// 이뒤에 바이트 데이터 InitialInGameData 가 있다.
};

struct NewUserPacket : PacketData
{
	InitialInGameData InitData;
};