using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class UIMessageBox : UIPopup
{
    public override PopupID GetPopupID()
    {
        return PopupID.UIMessageBox;
    }
    private Text m_Text = null;
    private void Awake()
    {
        m_Text = transform.Find("Text").GetComponent<Text>();
    }

    public void SetText(string _strText)
    {
        m_Text.text = _strText;
    }
    public static void ShowPopup(string _strText)
    {
        PopupManager.Instance.OpenPopup(PopupID.UIMessageBox, (popup) =>
        {
            UIMessageBox uIMessageBox = popup as UIMessageBox;
            if(uIMessageBox != null)
            {
                uIMessageBox.SetText(_strText);
            }
        });
    }
}
