using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using AddreesableManager;

public class IntroStart : MonoBehaviour
{
    private void Start()
    {
        StartCoroutine(IntroProcees());
    }

    private IEnumerator IntroProcees()
    {
        AddressableManager.Instance.Init();

        // ��巹���� �Ŵ����� �����Ҷ����� ���
        yield return AddressableManager.Instance.InitSuccessWait();

        PopupManager.Instance.Init();

        // ��巹���� �Ŵ����� �����Ҷ����� ���
        yield return PopupManager.Instance.InitSuccessWait();

        // ��������� ����ȯ
        SceneManager.Instance.Init();
        yield return SceneManager.Instance.InitSuccessWait();

        SceneManager.Instance.SceneChange(SceneType.Lobby);
    }
}
