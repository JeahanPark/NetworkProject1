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

    public IEnumerator CoRegister()
    {
        
        string strID = string.Empty;
        string strPassword = string.Empty;

        // UI 노출
        yield return UIRegister.ShowPopup((id, password) =>
        {
            strID = id;
            strPassword = password;
        });

        if(string.IsNullOrEmpty(strID) ||
            string.IsNullOrEmpty(strPassword))
        {
            Debug.Log("회원가입 안함");
            yield break;
        }


    }

    public override void Init()
    {

    }

    public override void Destroy()
    {

    }
}