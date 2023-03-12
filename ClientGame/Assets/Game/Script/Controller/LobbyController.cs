using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LobbyController : MonoDestroySingleton<LobbyController>
{
    private LobbyUIWorker m_LobbyUI = null;

    private void Awake()
    {
        
    }
    private void Start()
    {
        StartCoroutine(LobbyEnter());
    }

    private IEnumerator LobbyEnter()
    {
        yield return null;

//#if UNITY_EDITOR
//        AutoLogin();
//#endif
    }

    // 테스트 코드
    private void AutoLogin()
    {
        if (DataManager.Instance.IsLogin())
            return;

        Debug.Log("자동 테스트 로그인");

        SendLogIn("pdwer3376", "123123");
    }


    // 테스트 코드

    public void SetLobbyUIWorker(LobbyUIWorker lobby)
    {
        m_LobbyUI = lobby;
    }



    public void ReceiveChattingMessage(V2ChattingPacket _packet)
    {
        m_LobbyUI.ReceiveChattingMessage(_packet.chattingContent);
    }

    public void ReceiveRegister(ePacketResult _eResult)
    {
        if (_eResult == ePacketResult.Success)
            UIMessageBox.ShowPopup("회원가입 성공");
        else
            UIMessageBox.ShowPopup("회원가입 실패");
    }
    public void ReceiveLoginResult(LoginResultPacket _loginResult)
    {
        if(_loginResult.m_eResult == ePacketResult.Fail)
        {
            UIMessageBox.ShowPopup("로그인 실패");
            return;
        }

        DataManager.Instance.SetUserData(_loginResult);
        UIMessageBox.ShowPopup("로그인 성공");

        PopupManager.Instance.ClosePopup(PopupID.UILogIn);
        m_LobbyUI.ChattingButtonOn(true);
    }
    public void ReceiveChattingRoom(PacketResult _packetResult)
    {
        if (_packetResult.m_eResult == ePacketResult.Success)
        {
            if(_packetResult.m_SignalType == ePacketSignal.Signal_ChattingRoomEnter)
            {
                m_LobbyUI.ChattingRoomOn(true);
                m_LobbyUI.ChattingButtonOn(false);
            }
            else if(_packetResult.m_SignalType == ePacketSignal.Signal_ChattingRoomExit)
            {
                m_LobbyUI.ChattingRoomOn(false);
                m_LobbyUI.ChattingButtonOn(true);
            }
        }
        else if (_packetResult.m_eResult == ePacketResult.ChattingRoomEnter_Already_In)
            UIMessageBox.ShowPopup("이미 채널방에 있다.");
        else if (_packetResult.m_eResult == ePacketResult.ChattingRoomEnter_Not_Login)
            UIMessageBox.ShowPopup("로그인이 안되어있다");
        else if (_packetResult.m_eResult == ePacketResult.ChattingRoomExit_Not_Exist)
            UIMessageBox.ShowPopup("내가 방에 없다?");
    }

    public void ReceiveInGameEnter(PacketResult _packetResult)
    {
        // 데이터 세팅
        //DataManager.Instance.SetInteractionUserData (_packet.m_iInteractionIndex, _packet.m_eType);

        if(_packetResult.m_eResult == ePacketResult.Success)
        {
            // 인게임 진입
            SceneManager.Instance.SceneChange(SceneType.InGame);
        }
        else
        {
            // 실패
        }
    }

    public void ReceiveInGameFaile(PacketResult _packetResult)
    {
        if (_packetResult.m_eResult == ePacketResult.Success)
        {
            if (_packetResult.m_SignalType == ePacketSignal.Signal_InGameExit)
            {
                SceneManager.Instance.SceneChange(SceneType.Lobby);
            }
        }
        else if (_packetResult.m_eResult == ePacketResult.InGameEnter_Already_In)
            UIMessageBox.ShowPopup("이미 인게임에 있다.");
        else if (_packetResult.m_eResult == ePacketResult.InGameEnter_Not_Login)
            UIMessageBox.ShowPopup("로그인이 안되어있다.");
        else if (_packetResult.m_eResult == ePacketResult.InGameExit_Not_Exist)
            UIMessageBox.ShowPopup("내가 인게임에 없다.?");
        else if( _packetResult.m_eResult == ePacketResult.InGameEnter_InChattingRoom)
            UIMessageBox.ShowPopup("채팅방에서 나가야 인게임에 접근할수있다.");
    }
    public void SendChattingRoom(bool _bEnter)
    {
        if(!DataManager.Instance.IsLogin())
        {
            UIMessageBox.ShowPopup("로그인을 해주세요.");
            return;
        }

        SignalPacket packet = new SignalPacket();
        packet.m_ePacketSignal = _bEnter ? ePacketSignal.Signal_ChattingRoomEnter : ePacketSignal.Signal_ChattingRoomExit;

        Packet.SendPacket<SignalPacket>(packet, ePacketType.Signal);
    }

    public void SendInGameEnter(bool _bEnter)
    {
        if (!DataManager.Instance.IsLogin())
        {
            UIMessageBox.ShowPopup("로그인을 해주세요.");
            return;
        }

        SignalPacket packet = new SignalPacket();
        packet.m_ePacketSignal = _bEnter ? ePacketSignal.Signal_InGameEnter : ePacketSignal.Signal_InGameExit;

        Packet.SendPacket<SignalPacket>(packet, ePacketType.Signal);
    }

    public void SendRegister(string _strID, string _strPassword)
    {
        if(string.IsNullOrEmpty(_strID) ||
            string.IsNullOrEmpty(_strPassword))
        {
            Debug.Log("비밀번호나 아이디가 입력이안되있음");
            return;
        }
        UserRegistPacket packet = new UserRegistPacket();
        packet.m_UserID = _strID;
        packet.m_Password = _strPassword;

        Packet.SendPacket<UserRegistPacket>(packet, ePacketType.CToS_UserRegister);
    }
    public void SendLogIn(string _strID, string _strPassword)
    {
        if (string.IsNullOrEmpty(_strID) ||
            string.IsNullOrEmpty(_strPassword))
        {
            Debug.Log("비밀번호나 아이디가 입력이안되있음");
            return;
        }
        LogInPacket packet = new LogInPacket();
        packet.m_UserID = _strID;
        packet.m_Password = _strPassword;

        Packet.SendPacket<LogInPacket>(packet, ePacketType.CToS_Login);
    }
    public void SendChattingPacket(string _text)
    {
        V2ChattingPacket packet = new V2ChattingPacket();
        packet.chattingContent = _text;

        Packet.SendPacket<V2ChattingPacket>(packet, ePacketType.CToS_Chatting);
    }

    public override void Init()
    {

    }

    public override void Destroy()
    {
        base.Destroy();

        m_LobbyUI = null;
    }
}