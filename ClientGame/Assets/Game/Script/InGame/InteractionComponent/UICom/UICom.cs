using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UICom : BaseInteractionComponent
{
    private UserUI m_UserUI = null;
    public UICom(InteractionObject _ownerInteraction) : base(_ownerInteraction)
    {
        m_eInteractionCom = eInteractionCom.UI;
    }

    public override void InitComponent()
    {
        base.InitComponent();

        m_UserUI = InGameController.Instance.GetUIWorker.CreateUIUser(m_ownerInteraction.GetInteractionIndex);

        m_UserUI.Initialize(m_ownerInteraction.transform);
    }

    public override void ClearComponent()
    {
        base.ClearComponent();

        InGameController.Instance.GetUIWorker.PopUIUser(m_ownerInteraction.GetInteractionIndex);
        m_UserUI.ClearData();
        m_UserUI = null;
    }

    public void InitUI(string _strNickName)
    {
        m_UserUI.InitializeUI(_strNickName);
    }

    public void RefreshUI()
    {

    }
}
