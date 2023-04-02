using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ReflectionSkillEffect : BaseNormalEffect
{
    private Material m_materal = null;

    private bool m_bReflectionEffect = false;

    // 이거는 추가효과 이펙트 보여주는 시간
    private float m_fShowMaxTime = 0.4f;
    private float m_fShowCrtTime = 0;

    public ReflectionSkillEffect()
    {
        m_effectType = EffectType.ReflectionSkillEffect;
    }
    protected override void Awake()
    {
        base.Awake();

        // 이 이펙트를 관리하는애가 없애줄거다
        m_bStopAnimEnd = false;
        m_fMaxLifeTime = 3;

        m_materal = GetComponent<MeshRenderer>().material;
    }

    protected override void OnEnable()
    {
        base.OnEnable();

        DisableReflectionEffect();
    }


    public void ShowReflectionEffect(Vector3 _vDir)
    {
        float uvPosX = (_vDir.x + 1) / 2;
        float uvPosY = (_vDir.z + 1) / 2;

        Vector4 reciveDirUV = new Vector4(uvPosX, uvPosY, 1, 0);
        m_materal.SetVector("_ReciveDirUV", reciveDirUV);

        m_bReflectionEffect = true;
        m_fShowCrtTime = 0;
    }

    protected override void Update()
    {
        base.Update();

        // 카메라 바라보게함
        Vector3 dir = InGameController.Instance.GetCameraPosition - transform.position;
        dir.Normalize();

        transform.forward = dir;

        // 소유자의 자식으로 들어가있기 때문에 원점에서 계산한다.
        transform.localPosition = Vector3.zero;

        transform.position += dir * 2;

        if (m_bReflectionEffect)
        {
            m_fShowCrtTime += Time.deltaTime;
            if (m_fShowCrtTime > m_fShowMaxTime)
            {
                DisableReflectionEffect();
            }
        }
    }

    private void DisableReflectionEffect()
    {
        Vector4 reciveDirUV = new Vector4(0, 0, 0, 0);
        m_materal.SetVector("_ReciveDirUV", reciveDirUV);

        m_bReflectionEffect = false;
    }
}
