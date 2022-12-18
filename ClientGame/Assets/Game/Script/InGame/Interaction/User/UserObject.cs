using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UserObject : InteractionObject
{
    private bool MyInteraction
    {
        get
        {
            return DataManager.Instance.SameMyInteraction(m_iInteractionIndex, m_eInteractionType);
        }
    }

    public override void Initialize(eInteractionType _eInteractionType, int _iInteractionIndex)
    {
        base.Initialize(_eInteractionType, _iInteractionIndex);

        if(MyInteraction)
        {
            InGameController.Instance.SetMyCameraTarget(transform);
        }
    }

    public override void Clear()
    {
        base.Clear();

        if (MyInteraction)
        {
            InGameController.Instance.SetMyCameraTarget(null);
        }
    }
}
