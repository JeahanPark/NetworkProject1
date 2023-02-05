using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
public class UserUI : MonoBehaviour
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

    public void ClearData()
    {
        m_tranTarget = null;
        m_txtNickName.text = string.Empty;
    }

    private void Awake()
    {
        m_txtNickName = transform.Find<TextMeshProUGUI>("Text_NickName");
    }

    private void Update()
    {
        if (m_tranTarget == null)
            return;

        transform.position = Camera.main.WorldToScreenPoint(m_tranTarget.position);
    }



    private void RefreshUI()
    {

    }
}
