using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireBall : InteractionObject
{
    public override void Die()
    {
        InGameController.Instance.GetEffectWorker.GetPrefabEffect(EffectType.FireBallExplosionEffect, transform.position);
    }
}
