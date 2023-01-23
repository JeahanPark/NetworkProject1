using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameController : MonoDestroySingleton<InGameController>
{
    private InteractionWorker m_InteractionWorker = null;
    private InGameMyWorker m_InGameMyWorker = null;
    private InGameUIWorker m_InGameUIWorker = null;

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

    public void SetMyInteraction(UserObject _user)
    {
        m_InGameMyWorker.SetMyInteraction(_user);
    }

    public void SetInGameUIWorker(InGameUIWorker _ingameUIWorker)
    {
        m_InGameUIWorker = _ingameUIWorker;
    }



    #region PacketSend
    public void SendMyUserMove(Vector3 _vDir, float _fMoveSpeed)
    {
        MyUserMovePacket packet = new MyUserMovePacket();
        packet.m_vDir = _vDir;
        packet.m_fMoveSpeed = _fMoveSpeed;

        Packet.SendPacket<MyUserMovePacket>(packet, ePacketType.CToS_MyUserMove);
    }

    #endregion

    #region PacketReceive
    public void ReceiveInGameUpdate(InGameUpdatePacket _packet, InteractionPacketData[] _interactionPacketDatas)
    {
        m_InteractionWorker.UpdateInteraction(_packet, _interactionPacketDatas);
    }
    #endregion
}
