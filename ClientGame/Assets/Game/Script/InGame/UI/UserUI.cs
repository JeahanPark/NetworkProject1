using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class UserUI : MonoBehaviour
{
    private Text m_txtNickName = null;
    private Transform m_tranTarget = null;

    public void Initialize(Transform _target)
    {
        m_tranTarget = _target;
    }

    private void Awake()
    {
        m_txtNickName = transform.Find<Text>("Text_NickName");
    }

    private void Update()
    {
        if (m_tranTarget == null)
            return;

        transform.position = Camera.main.WorldToScreenPoint(m_tranTarget.position);
    }

    public void InitializeUI(string _strNickName)
    {
        m_txtNickName.text = _strNickName;
    }

    private void RefreshUI()
    {

    }
}
