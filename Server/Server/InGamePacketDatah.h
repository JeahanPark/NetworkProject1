#pragma once
#include "pch.h"


#pragma pack(1)
struct MyUserMovePacket : BasePacket
{
	XMFLOAT3	m_vMoveDir;
	float		m_fMoveSpeed;

	XMFLOAT3	m_vRotateY;
};
#pragma pack(1)
struct InteractionPacketData
{
	int						m_iInteractionIndex;
	eInteractionType		m_eType;
	bool					m_bValidLife;
	XMFLOAT3				m_vPos;
	XMFLOAT3				m_vMoveDir;
	XMFLOAT3				m_vRotateY;
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

#pragma pack(1)
struct UserRiseAgainPacket : BasePacket
{
	int		m_iInteractionIndex;
	bool	m_bRiseAgain;

	InitialInGameData InitData;
};

#pragma pack(1)
struct UpdateMySkillPacket : BasePacket
{
	eSkillType m_eSkillType;
};

#pragma pack(1)
struct UpdatePointPacket : BasePacket
{
	int m_iPoint;
};

#pragma pack(1)
struct ShowSkillPacket : BasePacket
{
	eSkillType m_eSkillType;
	int m_iInteractionIndex;
};