using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LobbyController : MonoDestroySingleton<LobbyController>
{
    private UILobby m_UILobby = null;

    private void Awake()
    {
        
    }

    public void SetUILobby(UILobby lobby)
    {
        m_UILobby = lobby;
    }

    public void InGameEnter()
    {
        SceneManager.Instance.SceneChange(SceneType.InGame);
    }

    public void Login(System.Action<bool> _funResult)
    {
        SocketManager.Instance.ServerConnect((result) => 
        {
            if(result)
            {
                DataManager.Instance.Login();
            }


            _funResult(result);
        });

        
    }

    public void ReceiveChattingMessage(V2ChattingPacket _packet)
    {
        m_UILobby.ReceiveChattingMessage(_packet.chattingContent);
    }

    public override void Init()
    {

    }

    public override void Destroy()
    {

    }
}