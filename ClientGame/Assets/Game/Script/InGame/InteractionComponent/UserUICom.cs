using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class UserUICom : MonoBehaviour
{
    private Text m_txtNickName = null;
    private Transform m_tranTarget = null;

    public void SetTarget(string _strNickName, Transform _target)
    {
        m_txtNickName.text = _strNickName;
        m_tranTarget = _target;
    }

    private void Awake()
    {
        m_txtNickName = transform.Find<Text>("txt_NickName");
    }

    private void Update()
    {
        if (m_tranTarget == null)
            return;

        transform.position = Camera.main.WorldToScreenPoint(m_tranTarget.position);
    }
}
