using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BaseEffect : MonoBehaviour
{
    protected EffectID m_effectID = EffectID.NONE;

    protected bool m_bStopAnimEnd = false;
    protected float m_fMaxLifeTime = 0;
    protected float m_fCrtLifeTime = 0;

    protected virtual void Awake()
    {
        ParticleSystem[] particle = GetComponentsInChildren<ParticleSystem>();

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

    public void StopEffect()
    {

    }

    private void OnEnable()
    {
        m_fCrtLifeTime = 0;
    }

    private void Update()
    {
        if(m_bStopAnimEnd)
        {
            m_fCrtLifeTime += Time.deltaTime;

            if(m_fMaxLifeTime > m_fCrtLifeTime)
            {
                StopEffect();
            }
        }
    }
}
