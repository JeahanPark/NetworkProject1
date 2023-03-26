using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum eInteractionType
{
    None,
    User,
    AttackDummy,
    AttackFireBall,
    Wall,
    ReflectionItem,
    Interaction_MAX
}
public enum EffectType
{
    NONE,
    UserBody_Normal,
    UserBody_Damage,

    PrefabEffect,
    DieEffect,
    UserCreateEffect,
    FireBallExplosionEffect,
    ReflectionItemEffect,
    Effect_MAX
}
public enum eSkillType
{
    None,
	FireBall,
	Wall,
    Reflection,
    Max
};

public static class EnumType
{
    // 어케해야될지... 모르겟음
    public const int maxInteractionCount = 10;
    public const int InteractionIndexNone = -1;


    public const int BUFF_MAX_SIZE = 65535;

    public const float m_f10Frame = 0.1666666666666667f;
}

