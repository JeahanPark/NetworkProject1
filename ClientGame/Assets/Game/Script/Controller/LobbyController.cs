using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LobbyController : MonoDestroySingleton<LobbyController>
{
    private UILobby m_UILobby = null;

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
    }

    public void SetUILobby(UILobby lobby)
    {
        m_UILobby = lobby;
    }

    public void InGameEnter()
    {
        SceneManager.Instance.SceneChange(SceneType.InGame);
    }

    public void ServerConnect(System.Action<bool> _funResult)
    {
        SocketManager.Instance.ServerConnect((result) => 
        {
            _funResult(result);
        });
    }

    public void ReceiveChattingMessage(V2ChattingPacket _packet)
    {
        m_UILobby.ReceiveChattingMessage(_packet.chattingContent);
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
        m_UILobby.ActiveChatting();
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

    }
}