using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIUserRiseAgain : UIPopup
{
    public override PopupID GetPopupID()
    {
        return PopupID.UIUserRiseAgain;
    }

    public void OnClickRise()
    {
        InGameController.Instance.SendUserRiseAgain();
    }
}
