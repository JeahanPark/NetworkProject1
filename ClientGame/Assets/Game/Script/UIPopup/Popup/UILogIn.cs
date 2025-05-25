using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UILogIn : UIPopup
{
    private InputField m_ID = null;
    private InputField m_Password = null;

    public override PopupID GetPopupID()
    {
        return PopupID.UILogIn;
    }


    private void Awake()
    {
        m_ID = transform.Find("InputField").GetComponent<InputField>();
        m_Password = transform.Find("InputField (1)").GetComponent<InputField>();
    }

    public void OnClickLogIn()
    {
        //if (m_ID.text.Length == 0)
        //{
        //    UIMessageBox.ShowPopup("아이디 쓰세요");
        //    return;
        //}

        //if (m_Password.text.Length == 0)
        //{
        //    UIMessageBox.ShowPopup("비밀번호 쓰세요");
        //    return;
        //}

        //LobbyController.Instance.SendLogIn(m_ID.text, m_Password.text);
    }

    public static void ShowPopup()
    {
        PopupManager.Instance.OpenPopup(PopupID.UILogIn, (popup) =>
        {
        });
    }
}
