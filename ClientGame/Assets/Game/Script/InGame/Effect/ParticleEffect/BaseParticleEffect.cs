using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BaseParticleEffect : BaseEffect
{
    protected ParticleSystem[] particle = null;

    protected override void Awake()
    {
        base.Awake();
        particle = GetComponentsInChildren<ParticleSystem>();

        for (int i = 0; i < particle.Length; ++i)
        {
            // ��ƼŬ �ý����� ��� �ѽð�
            if (particle[i].main.duration > m_fMaxLifeTime)
            {
                m_fMaxLifeTime = particle[i].main.duration;
            }

            //��ƼŬ ������ �ִ� �ð� Duration������ �ִ�.
            if (particle[i].main.startLifetime.constantMax > m_fMaxLifeTime)
            {
                m_fMaxLifeTime = particle[i].main.duration;
            }
        }
    }
    public override void StopEffect()
    {
        base.StopEffect();
        for (int i = 0; i < particle.Length; ++i)
        {
            particle[i].Stop();
        }
    }
    protected override void OnEnable()
    {
        base.OnEnable();
        for (int i = 0; i < particle.Length; ++i)
        {
            particle[i].Play();
        }
    }
}
