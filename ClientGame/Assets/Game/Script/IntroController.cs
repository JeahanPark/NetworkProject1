using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using AddreesableManager;

public class IntroController : MonoBehaviour
{
    private void Start()
    {
        StartCoroutine(IntroProcees());
    }

    private IEnumerator IntroProcees()
    {
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

        // ��������� ����ȯ
        SceneManager.Instance.Init();
        yield return SceneManager.Instance.InitSuccessWait();

        SceneManager.Instance.SceneChange(SceneType.Lobby);
    }
}
