using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIChatting : MonoBehaviour
{
    private const int m_iMaxInputText = 20;

    private InputField m_InputField = null;
    private Text m_txtPrefabs = null;
    private Transform m_transContent = null;
    private void Awake()
    {
        m_InputField = transform.Find("InputField").GetComponent<InputField>();

        m_txtPrefabs = transform.Find("Scroll View/Viewport/Content/Text").GetComponent<Text>();

        m_transContent = transform.Find("Scroll View/Viewport/Content");

    }

    private void Start()
    {
        m_InputField.onValueChanged.AddListener(OnClickInputText);

        Button _Send = transform.Find("Send").GetComponent<Button>();
        _Send.onClick.AddListener(OnClickSendMessage);

    }
    private void OnClickInputText(string _strText)
    {
        if (string.IsNullOrEmpty(_strText))
            return;

        if(_strText.Length > m_iMaxInputText)
        {
            UIMessageBox.ShowMessageBox("21자 이상은 텍스트 입력이 불가능합니다.");

            m_InputField.text = _strText.Substring(0, m_iMaxInputText);
        }
    }


    private void OnClickSendMessage()
    {
        if (string.IsNullOrEmpty(m_InputField.text))
            return;

        // 어차피 동기라 블락킹이라 상관없긴한데... 나중에 바꿔야겠다
        SocketManager.Instance.Send(m_InputField.text);
    }

    public void ReceiveChattingMessage(string _sendMessage)
    {
        Text text = Instantiate<Text>(m_txtPrefabs, m_transContent);
        text.gameObject.SetActive(true);
        text.text = _sendMessage;
    }
}
