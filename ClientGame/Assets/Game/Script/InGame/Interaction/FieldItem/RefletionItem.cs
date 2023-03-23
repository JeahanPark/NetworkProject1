using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RefletionItem : InteractionObject
{
    public override void Die()
    {
        InGameController.Instance.GetEffectWorker.GetPrefabEffect(EffectType.ReflectionItemEffect, transform.position);
    }
}
