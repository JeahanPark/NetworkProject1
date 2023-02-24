using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class DamageUI : BaseUI
{
    private TextMeshProUGUI m_txtDamage = null;

    private float m_fShowCrtTime = 0;
    private const float m_fShowMaxTime = 1;

    public void InitUI(float _fDamage, Vector3 _vPos)
    {
        m_txtDamage.text = string.Format("-{0}", _fDamage);
        m_fShowCrtTime = 0;

        transform.position = ChangePosWorldToUI(_vPos);
    }
    protected override void Awake()
    {
        base.Awake();
        m_txtDamage = transform.Find<TextMeshProUGUI>("Text_NickName");
    }

    private void Update()
    {
        if(m_fShowCrtTime < m_fShowMaxTime)
        {
        }
    }
}
