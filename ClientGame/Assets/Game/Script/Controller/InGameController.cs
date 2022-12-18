using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameController : MonoDestroySingleton<InGameController>
{
    private InteractionWorker m_InteractionWorker = null;
    private InGameMyWorker m_InGameMyWorker = null;
    public override void Destroy()
    {
        base.Destroy();
    }

    public override void Init()
    {

    }

    private void Start()
    {
        Init();
    }

    public void SetInteractionWorker(InteractionWorker _interactionWorker)
    {
        m_InteractionWorker = _interactionWorker;
    }

    public void SetInGameWorker(InGameMyWorker _ingameMyWorker)
    {
        m_InGameMyWorker = _ingameMyWorker;
    }

    public void ReceiveInGameUpdate(InGameUpdatePacket _packet)
    {
        if (m_InteractionWorker == null)
            return;

        m_InteractionWorker.UpdateInteraction(_packet);
    }

    public void SetMyCameraTarget(Transform _target)
    {
        m_InGameMyWorker.SetTarget(_target);
    }
}
