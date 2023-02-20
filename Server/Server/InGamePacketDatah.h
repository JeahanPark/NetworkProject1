#pragma once
#include "pch.h"


#pragma pack(1)
struct MyUserMovePacket : BasePacket
{
	XMFLOAT3	m_vDir;
	float		m_fMoveSpeed;
};
#pragma pack(1)
struct InteractionPacketData
{
	int						m_iInteractionIndex;
	eInteractionType		m_eType;
	bool					m_bValidLife;
	XMFLOAT3				m_vPos;
	XMFLOAT3				m_vDir;
	float					m_fMoveSpeed;
};
#pragma pack(1)
struct InGameUpdatePacket : BasePacket
{
	int						m_iInteractionCount;
	// 이뒤에 바이트 데이터 InteractionPacketData 가 있다.
};
#pragma pack(1)
struct InitialInGameData : InteractionPacketData
{
	WCHAR					m_UserID[USER_ID_LENGTH] = {};
};
#pragma pack(1)
struct InitialInGameDataPacket : BasePacket
{
	int						m_iMyInteractionIndex;
	int						m_iUserCount;
	// 이뒤에 바이트 데이터 InitialInGameData 가 있다.
};
#pragma pack(1)
struct NewUserPacket : BasePacket
{
	InitialInGameData InitData;
};
#pragma pack(1)
struct RecivedDamagePacket : BasePacket
{
	int						m_iInteractionIndex;
	float					m_fReciveDamage;
};