using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UILobby : MonoBehaviour
{
    private UIChatting m_Chatting = null;


    private void Awake()
    {
        m_Chatting = transform.Find("LeftMiddle/Chatting").GetComponent<UIChatting>();
        m_Chatting.gameObject.SetActive(false);
    }

    private void Start()
    {
        LobbyController.Instance.SetUILobby(this);
    }

    public void OnClickInGameEnter()
    {
        LobbyController.Instance.InGameEnter();
    }

    public void OnClickLogin()
    {
        UILogIn.ShowPopup();
    }

    public void OnClickRegister()
    {
        UIRegister.ShowPopup();
    }

    public void ActiveChatting()
    {
        // �α������ؾ� ä��â�� �����ִ�!
        m_Chatting.gameObject.SetActive(DataManager.Instance.IsLogin());
    }

    public void ReceiveChattingMessage(string strMessage)
    {
        m_Chatting.ReceiveChattingMessage(strMessage);
    }
}
