using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UserCreateEffect : BaseEffect
{
    public UserCreateEffect()
    {
        m_effectType = EffectType.UserCreateEffect;
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
