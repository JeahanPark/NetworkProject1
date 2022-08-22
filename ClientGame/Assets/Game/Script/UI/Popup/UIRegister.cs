using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class UIRegister : UIPopup
{
    private InputField m_ID = null;
    private InputField m_Password = null;
    private System.Action<string, string> m_funResult;

    public bool m_bRegister = false;
    public override PopupID GetPopupID()
    {
        return PopupID.UIRegister;
    }

    public void SetRegisterSuccessFun(System.Action<string, string> result)
    {
        m_funResult = result;
    }

    private void Awake()
    {
        m_ID = transform.Find("InputField").GetComponent<InputField>();
        m_Password = transform.Find("InputField (1)").GetComponent<InputField>();
    }

    public void OnClickRegister()
    {
        if (m_ID.text.Length > 0)
        {
            UIMessageBox.ShowPopup("아이디 쓰세요");
            return;
        }

        if (m_Password.text.Length > 0)
        {
            UIMessageBox.ShowPopup("비밀번호 쓰세요");
            return;
        }

        m_funResult(m_ID.text, m_Password.text);
    }

    public static IEnumerator ShowPopup(System.Action<string, string> result)
    {
        bool bResult = false;
        UIRegister uIRegister = null;
        PopupManager.Instance.OpenPopup(PopupID.UIMessageBox, (popup) =>
        {
            bResult = true;
            uIRegister = popup as UIRegister;
            if (uIRegister != null)
            {
                uIRegister.SetRegisterSuccessFun(result);
            }
        });

        while (!bResult || !uIRegister.IsClose)
            yield return null;

        if(!uIRegister.m_bRegister || )
    }
}
