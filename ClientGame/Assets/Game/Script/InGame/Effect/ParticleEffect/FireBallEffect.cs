using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireBallEffect : BaseParticleEffect
{
    public FireBallEffect()
    {
        m_effectType = EffectType.FireBallExplosionEffect;
    }

    protected override void Awake()
    {
        base.Awake();
        m_bStopAnimEnd = true;
    }
}
