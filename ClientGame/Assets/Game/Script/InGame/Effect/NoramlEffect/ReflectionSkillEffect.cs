using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ReflectionSkillEffect : BaseNormalEffect
{
    private Material m_materal = null;

    private bool m_bReflectionEffect = false;

    // �̰Ŵ� �߰�ȿ�� ����Ʈ �����ִ� �ð�
    private float m_fShowMaxTime = 0.4f;
    private float m_fShowCrtTime = 0;

    public ReflectionSkillEffect()
    {
        m_effectType = EffectType.ReflectionSkillEffect;
    }
    protected override void Awake()
    {
        base.Awake();

        // �� ����Ʈ�� �����ϴ¾ְ� �����ٰŴ�
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

        // ī�޶� �ٶ󺸰���
        Vector3 dir = InGameController.Instance.GetCameraPosition - transform.position;
        dir.Normalize();

        transform.forward = dir;

        // �������� �ڽ����� ���ֱ� ������ �������� ����Ѵ�.
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
