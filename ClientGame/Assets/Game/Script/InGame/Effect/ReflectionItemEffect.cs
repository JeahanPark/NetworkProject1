using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ReflectionItemEffect : BaseEffect
{
    public ReflectionItemEffect()
    {
        m_effectType = EffectType.ReflectionItemEffect;
    }

    protected override void Awake()
    {
        base.Awake();
        m_bStopAnimEnd = true;
    }
}
