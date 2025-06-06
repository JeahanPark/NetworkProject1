using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ReflectionItemEffect : BaseParticleEffect
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
