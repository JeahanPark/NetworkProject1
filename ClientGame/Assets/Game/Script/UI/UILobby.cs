using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UILobby : MonoBehaviour
{
    private UIChatting m_Chatting = null;
    private GameObject m_goChattingButton = null;


    private void Awake()
    {
        m_Chatting = transform.Find("LeftMiddle/Chatting").GetComponent<UIChatting>();
        m_goChattingButton = transform.Find("LeftBottom/BtnList/Btn_Chatting").gameObject;

        m_Chatting.gameObject.SetActive(false);
        ActiveChattingButton();
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

    public void OnClickChattingRoomEnter()
    {
        if(m_Chatting.gameObject.activeSelf)
        {
            UIMessageBox.ShowPopup("이미 채팅팝업이 켜져있다.");
            return;
        }

        LobbyController.Instance.SendChattingRoom(true);
    }

    public void ActiveChattingButton()
    {
        m_goChattingButton.SetActive(DataManager.Instance.IsLogin());
    }

    public void ChattingRoomOn(bool _On)
    {
        m_Chatting.gameObject.SetActive(_On);
    }

    public void ReceiveChattingMessage(string strMessage)
    {
        m_Chatting.ReceiveChattingMessage(strMessage);
    }
}
