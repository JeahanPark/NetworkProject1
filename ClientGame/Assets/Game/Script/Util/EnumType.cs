using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public enum eInteractionType
{
    None,
    User,
    AttackDummy,
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
    Effect_MAX
}
public static class EnumType
{
    // 어케해야될지... 모르겟음
    public const int maxInteractionCount = 10;
    public const int InteractionIndexNone = -1;


    public const int BUFF_MAX_SIZE = 65535;
}

