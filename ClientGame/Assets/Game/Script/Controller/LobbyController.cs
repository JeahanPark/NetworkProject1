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
            UIMessageBox.ShowPopup("ȸ������ ����");
        else
            UIMessageBox.ShowPopup("ȸ������ ����");
    }

    public void SendRegister(string _strID, string _strPassword)
    {
        if(string.IsNullOrEmpty(_strID) ||
            string.IsNullOrEmpty(_strPassword))
        {
            Debug.Log("��й�ȣ�� ���̵� �Է��̾ȵ�����");
            return;
        }
        UserRegistPacket packet = new UserRegistPacket();
        packet.m_UserID = _strID;
        packet.m_Password = _strPassword;

        Packet.SendPacket<UserRegistPacket>(packet, ePacketType.CToS_UserRegister);
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