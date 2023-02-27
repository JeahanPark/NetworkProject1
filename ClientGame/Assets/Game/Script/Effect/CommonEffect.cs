using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CommonEffect : BaseEffect
{
    protected override void Awake()
    {
        base.Awake();
        m_bStopAnimEnd = true;
    }
}
