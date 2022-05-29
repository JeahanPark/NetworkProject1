using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UINickNameSetting : UIPopup
{
    private InputField m_InputField = null;


    public override PopupID GetPopupID()
    {
        return PopupID.UINickNameSetting;
    }


    private void Awake()
    {
        m_InputField = transform.Find("InputField").GetComponent<InputField>();

    }
}
