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

        // 어드레서블 매니저가 성공할때까지 대기
        yield return AddressableManager.Instance.InitSuccessWait();

        PopupManager.Instance.Init();

        // 어드레서블 매니저가 성공할때까지 대기
        yield return PopupManager.Instance.InitSuccessWait();

        // 끝났을경우 씬전환
        SceneManager.Instance.Init();
        yield return SceneManager.Instance.InitSuccessWait();

        SceneManager.Instance.SceneChange(SceneType.Lobby);
    }
}
