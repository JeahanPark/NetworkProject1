using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
public class UserUI : BaseUI
{
    private TextMeshProUGUI m_txtNickName = null;
    private Transform m_tranTarget = null;

    public void Initialize(Transform _target)
    {
        m_tranTarget = _target;
    }
    public void InitializeUI(string _strNickName)
    {
        m_txtNickName.text = _strNickName;
    }

    public override void ClearData()
    {
        m_tranTarget = null;
        m_txtNickName.text = string.Empty;
    }

    protected override void Awake()
    {
        base.Awake();
        m_txtNickName = transform.Find<TextMeshProUGUI>("Text_NickName");
    }

    private void Update()
    {
        if (m_tranTarget == null)
            return;


        transform.position = ChangePosWorldToUI(m_tranTarget.position);
    }



    private void RefreshUI()
    {

    }
}
