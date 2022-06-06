using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LobbyController : MonoBehaviour
{
    private void Start()
    {
        UIMessageBox.ShowMessageBox("�׽�Ʈ");
    }

    public void InGameEnter()
    {
        SceneManager.Instance.SceneChange(SceneType.InGame);
    }

    public void Login()
    {
        SocketManager.Instance.Init();

        SocketManager.Instance.Send();
    }
}