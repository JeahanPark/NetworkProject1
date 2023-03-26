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

    public InGameMyWorker GetMyWorker
    {
        get
        {
            return m_InGameMyWorker;
        }
    }

    public override void Destroy()
    {
        base.Destroy();
    }

    public override void Init()
    {

    }

    private void Start()
    {
        StartCoroutine(CoInit());
    }
    private IEnumerator CoInit()
    {
        // 모든 worker세팅은 awake시점에서 세팅한다.

        // 이펙트 세팅
        yield return m_InGameEffectWorker.InitProcessor();

        SignalPacket packet = new SignalPacket();
        packet.m_ePacketSignal = ePacketSignal.Signal_InitialInGameData;

        Packet.SendPacket<SignalPacket>(packet, ePacketType.Signal);

        _fUpdateLatency = 0;
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
    public void SendMyUserMove(Vector3 _vMoveDir, Vector3 _vRotateY, float _fMoveSpeed)
    {
        MyUserMovePacket packet = new MyUserMovePacket();
        packet.m_vMoveDir = _vMoveDir;
        packet.m_fMoveSpeed = _fMoveSpeed;
        packet.m_vRotateY = _vRotateY;

        Packet.SendPacket<MyUserMovePacket>(packet, ePacketType.CToS_MyUserMove);
    }

    public void SendUserRiseAgain()
    {
        SignalPacket packet = new SignalPacket();
        packet.m_ePacketSignal = ePacketSignal.Signal_InGameUserRiseAgain;

        Packet.SendPacket<SignalPacket>(packet, ePacketType.Signal);
    }

    public void SendAttack()
    {
        SignalPacket packet = new SignalPacket();
        packet.m_ePacketSignal = ePacketSignal.Signal_InGameAttack;

        Packet.SendPacket<SignalPacket>(packet, ePacketType.Signal);
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

    public void ReciveUserRiseAgain(UserRiseAgainPacket _packet)
    {
        if(!_packet.RiseAgain)
        {
            //못살아나
            return;
        }

        //살아나
        UIPopup popup = PopupManager.Instance.GetOpenPopup(PopupID.UIUserRiseAgain);
        if(popup != null)
            popup.Close();

        m_InteractionWorker.UserRiseAgain(_packet.m_myInitData);
    }
    public void ReciveUpdatetMySkill(UpdatetMySkillPacket _packet)
    {
        DataManager.Instance.SetSkillType(_packet.m_eSkillType);

        SygnalManager.Instance.Fire(SygnalEventType.InGame_UpdateMySkill);
    }

    public void ReciveUpdatePoint(UpdatePointPacket _packet)
    {
        GetMyWorker.GetMyData.UpdatePoint(_packet.m_iPoint);

        SygnalManager.Instance.Fire(SygnalEventType.InGame_UpdatePoint);
    }
    #endregion
}
