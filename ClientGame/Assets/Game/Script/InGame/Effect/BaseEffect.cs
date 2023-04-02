using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BaseEffect : MonoBehaviour
{
    protected EffectType m_effectType = EffectType.NONE;
    public EffectType GetEffectType
    {
        get
        {
            return m_effectType;
        }
    }

    // 애니메이션이 끝나면 사라진다.
    protected bool m_bStopAnimEnd = true;

    protected float m_fMaxLifeTime = 0;
    protected float m_fCrtLifeTime = 0;



    protected virtual void Awake()
    {
    }

    public virtual void StopEffect()
    {
        InGameController.Instance.GetEffectWorker.UnUseEffect(this);
    }

    protected virtual void OnEnable()
    {
        m_fCrtLifeTime = 0;

    }

    public bool UpdateAnim()
    {
        m_fCrtLifeTime += Time.deltaTime;

        if (m_fMaxLifeTime < m_fCrtLifeTime)
        {
            StopEffect();
            return false;
        }
        return true;
    }

    protected virtual void Update()
    {
        if(m_bStopAnimEnd)
        {
            UpdateAnim();
        }
    }
}
