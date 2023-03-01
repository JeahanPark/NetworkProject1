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
    protected bool m_bStopAnimEnd = true;
    protected float m_fMaxLifeTime = 0;
    protected float m_fCrtLifeTime = 0;

    protected ParticleSystem[] particle = null;

    protected virtual void Awake()
    {
        particle = GetComponentsInChildren<ParticleSystem>();

        for( int i = 0; i < particle.Length; ++i)
        {
            // 파티클 시스템의 재생 총시간
            if(particle[i].main.duration > m_fMaxLifeTime)
            {
                m_fMaxLifeTime = particle[i].main.duration;
            }

            //파티클 입자의 최대 시간 Duration끝나도 있다.
            if (particle[i].main.startLifetime.constantMax > m_fMaxLifeTime)
            {
                m_fMaxLifeTime = particle[i].main.duration;
            }
        }
    }
    public void InitEffect(bool _bStopAnimEnd)
    {
        m_bStopAnimEnd = _bStopAnimEnd;
    }

    public virtual void StopEffect()
    {
        for (int i = 0; i < particle.Length; ++i)
        {
            particle[i].Stop();
        }
        InGameController.Instance.GetEffectWorker.UnUseEffect(this);
    }

    private void OnEnable()
    {
        m_fCrtLifeTime = 0;
        for( int i = 0; i < particle.Length; ++i)
        {
            particle[i].Play();
        }
    }

    private void Update()
    {
        if(m_bStopAnimEnd)
        {
            m_fCrtLifeTime += Time.deltaTime;

            if(m_fMaxLifeTime < m_fCrtLifeTime)
            {
                StopEffect();
            }
        }
    }
}
