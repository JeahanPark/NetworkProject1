using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class UIRegister : UIPopup
{
    private InputField m_ID = null;
    private InputField m_Password = null;

    public override PopupID GetPopupID()
    {
        return PopupID.UIRegister;
    }


    private void Awake()
    {
        m_ID = transform.Find("InputField").GetComponent<InputField>();
        m_Password = transform.Find("InputField (1)").GetComponent<InputField>();
    }

    public void OnClickRegister()
    {
        if (m_ID.text.Length == 0)
        {
            UIMessageBox.ShowPopup("���̵� ������");
            return;
        }

        if (m_Password.text.Length == 0)
        {
            UIMessageBox.ShowPopup("��й�ȣ ������");
            return;
        }

        LobbyController.Instance.SendRegister(m_ID.text, m_Password.text);
    }

    public static void ShowPopup()
    {
        PopupManager.Instance.OpenPopup(PopupID.UIRegister, (popup) =>
        {
        });
    }
}
