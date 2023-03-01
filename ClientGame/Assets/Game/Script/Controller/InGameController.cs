using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameController : MonoDestroySingleton<InGameController>
{
    private InteractionWorker m_InteractionWorker = null;
    private InGameMyWorker m_InGameMyWorker = null;
    private InGameUIWorker m_InGameUIWorker = null;
    private InGameEffectWorker m_InGameEffectWorker = null;
    public InGameUIWorker GetUIWorker
    {
        get
        {
            return m_InGameUIWorker;
        }
    }
    public InGameEffectWorker GetEffectWorker
    {
        get
        {
            return m_InGameEffectWorker;
        }
    }

    public override void Destroy()
    {
        base.Destroy();
    }

    public override void Init()
    {
        // 모든 worker세팅은 awake시점에서 세팅한다.
        SignalPacket packet = new SignalPacket();
        packet.m_ePacketSignal = ePacketSignal.Signal_InitialInGameData;

        Packet.SendPacket<SignalPacket>(packet, ePacketType.Signal);

        _fUpdateLatency = 0;
    }

    private void Start()
    {
        Init();
    }
    private float _fUpdateLatency = 0;
    private void Update()
    {
        _fUpdateLatency += Time.deltaTime;
    }
    public void SetInteractionWorker(InteractionWorker _interactionWorker)
    {
        m_InteractionWorker = _interactionWorker;
    }

    public void SetInGameMyWorker(InGameMyWorker _ingameMyWorker)
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

    public void SetInGameEffectWorker(InGameEffectWorker _effectWorker)
    {
        m_InGameEffectWorker = _effectWorker;
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
    public void ReceiveInGameUpdate(InGameUpdatePacket _packet, InteractionData[] _interactionPacketDatas)
    {
        m_InteractionWorker.UpdateInteraction(_packet, _interactionPacketDatas, _fUpdateLatency * 0.5f);
        _fUpdateLatency = 0;
    }

    public void ReceiveInitialInGameData(InitialInGameDataPacket _packet, InitialInGameData[] _interationInitDatas)
    {
        m_InteractionWorker.SetInitialInGameData(_packet, _interationInitDatas);
    }

    public void ReceiveNewUser(NewUserPacket _packet)
    {
        m_InteractionWorker.AddNewUser(_packet);
    }

    public void RecivedDamage(RecivedDamagePacket _packet)
    {
        m_InteractionWorker.RecivedDamage(_packet);
    }
    #endregion
}
