using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using AddreesableManager;
using System.Runtime.InteropServices;

public class IntroController : MonoBehaviour
{
    private void Start()
    {
        StartCoroutine(IntroProcees());
    }

    private IEnumerator IntroProcees()
    {
        GameSetting();

        // ��巹���� �Ŵ����� �����Ҷ����� ���
        {
            AddressableManager.Instance.Init();

            yield return AddressableManager.Instance.InitSuccessWait();
        }


        // �˾� �Ŵ����� �ʱ�ȭ
        {
            PopupManager.Instance.Init();

            yield return PopupManager.Instance.InitSuccessWait();
        }

        // ���� ����
        {
            bool bResult = false;
            LobbyController.Instance.ServerConnect((result) =>
            {
                bResult = result;
            });

            while (!bResult)
                yield return null;
        }


        // ��������� ����ȯ
        SceneManager.Instance.Init();
        yield return SceneManager.Instance.InitSuccessWait();

        SceneManager.Instance.SceneChange(SceneType.Lobby);
    }

    private void GameSetting()
    {
        Application.targetFrameRate = 60;
    }
}
