using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UICom : BaseInteractionComponent
{
    private UserInfoCom _userInfo = null;

    public UICom(InteractionObject _ownerInteraction) : base(_ownerInteraction)
    {
        m_eInteractionCom = eInteractionCom.UI;
    }

    public override void InitComponent()
    {
        base.InitComponent();
    }
}
