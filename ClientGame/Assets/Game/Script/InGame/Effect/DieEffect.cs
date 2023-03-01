using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DieEffect : BaseEffect
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

    public override void StopEffect()
    {
        base.StopEffect();
        InGameController.Instance.GetEffectWorker.UnUseEffect(this);
    }
}
