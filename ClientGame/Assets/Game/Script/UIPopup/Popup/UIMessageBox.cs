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
    private System.Action m_funResult;
    private void Awake()
    {
        m_Text = transform.Find("Text").GetComponent<Text>();
    }

    public override void Close()
    {
        m_funResult?.Invoke();
        base.Close();
    }

    public void InitMessageBox(string _strText, System.Action _funResult)
    {
        m_Text.text = _strText;
        m_funResult = _funResult;
    }
    public static void ShowPopup(string _strText, System.Action _funResult = null)
    {
        PopupManager.Instance.OpenPopup(PopupID.UIMessageBox, (popup) =>
        {
            UIMessageBox uIMessageBox = popup as UIMessageBox;
            if(uIMessageBox != null)
            {
                uIMessageBox.InitMessageBox(_strText, _funResult);
            }
        });
    }
}
