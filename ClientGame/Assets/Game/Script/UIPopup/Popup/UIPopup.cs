using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class UIPopup : MonoBehaviour
{
    public abstract PopupID GetPopupID();

    public virtual void Close()
    {
        PopupManager.Instance.ClosePopup(GetPopupID());
    }

    public bool IsClose
    {
        get
        {
            return !gameObject.activeSelf;
        }
    }
}
