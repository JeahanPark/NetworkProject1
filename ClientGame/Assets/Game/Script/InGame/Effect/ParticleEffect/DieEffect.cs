using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DieEffect : BaseParticleEffect
{
    public DieEffect()
    {
        m_effectType = EffectType.DieEffect;
    }
    protected override void Awake()
    {
        base.Awake();
        m_bStopAnimEnd = true;
    }
}
