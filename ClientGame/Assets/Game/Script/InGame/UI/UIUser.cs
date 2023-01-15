using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class UIUser : MonoBehaviour
{
    private Text m_txtNickName = null;
    
    
    private Transform m_tranTarget = null;
    private InfoCom m_infoCom = null;

    public void Initialize(Transform _target, InfoCom _infoCom)
    {
        m_infoCom = _infoCom;
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

    private void InitializeUI()
    {
        //m_txtNickName.text = 
    }

    private void RefreshUI()
    {

    }
}
