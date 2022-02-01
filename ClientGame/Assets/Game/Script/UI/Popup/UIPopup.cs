using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class UIPopup : UIMonoBehaviour
{
    public abstract PopupID GetPopupID();

    public virtual void Close()
    {
        PopupManager.Instance.ClosePopup(GetPopupID());
    }
}
