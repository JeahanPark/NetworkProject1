using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIChatting : MonoBehaviour
{
    private const int m_iMaxInputText = 20;
    private const int m_iMaxChatting = 30;

    private InputField          m_InputField = null;
    private Text                m_txtPrefabs = null;
    private Transform           m_transContent = null;
    private ScrollRect          m_scroll = null;
    private void Awake()
    {
        m_InputField = transform.Find("InputField").GetComponent<InputField>();

        m_txtPrefabs = transform.Find("Text").GetComponent<Text>();

        m_transContent = transform.Find("Scroll View/Viewport/Content");

        m_scroll = transform.Find("Scroll View").GetComponent<ScrollRect>();
    }
    private void OnEnable()
    {
        if (m_transContent.childCount == 0)
            return;
        foreach (Transform child in m_transContent.GetComponentsInChildren<Transform>())
        {
            // �ƴ� �θ� ��ã�°�
            if (child == m_transContent)
                continue;
            child.gameObject.SetActive(false);
        }
    }

    private void OnDestroy()
    {
        if(gameObject.activeSelf)
            LobbyController.Instance.SendChattingRoom(false);
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
            UIMessageBox.ShowPopup("21�� �̻��� �ؽ�Ʈ �Է��� �Ұ����մϴ�.");

            m_InputField.text = _strText.Substring(0, m_iMaxInputText);
        }
    }


    private void OnClickSendMessage()
    {
        if (string.IsNullOrEmpty(m_InputField.text))
            return;

        LobbyController.Instance.SendChattingPacket(m_InputField.text);

        m_InputField.text = string.Empty;

        m_InputField.ActivateInputField();
    } 

    public void ReceiveChattingMessage(string _sendMessage)
    {
        if(m_transContent.childCount <= m_iMaxChatting)
        {
            // �ִ� 30�������� �����.
            Text text = Instantiate<Text>(m_txtPrefabs, m_transContent);
            text.gameObject.SetActive(true);
            text.name = m_transContent.childCount.ToString();
            text.text = _sendMessage;

            m_scroll.verticalNormalizedPosition = 0;
        }
        else
        {
            // ����Ʈ���� ������ �ε��� ���Ҹ� ù��°�� �ٲ� ����
            Text text = m_transContent.GetChild(0).GetComponent<Text>();
            text.gameObject.SetActive(true);
            text.transform.SetAsLastSibling();
            text.text = _sendMessage;
        }
    }

    public void OnClickClose()
    {
        LobbyController.Instance.SendChattingRoom(false);
    }
}
