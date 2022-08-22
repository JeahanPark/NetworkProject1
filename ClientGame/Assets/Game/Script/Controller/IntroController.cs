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
        // 어드레서블 매니저가 성공할때까지 대기
        {
            AddressableManager.Instance.Init();

            yield return AddressableManager.Instance.InitSuccessWait();
        }


        // 팝업 매니저가 초기화
        {
            PopupManager.Instance.Init();

            yield return PopupManager.Instance.InitSuccessWait();
        }

        // 소켓 연결
        {
            bool bResult = false;
            LobbyController.Instance.ServerConnect((result) =>
            {
                bResult = result;
            });

            while (!bResult)
                yield return null;
        }


        // 끝났을경우 씬전환
        SceneManager.Instance.Init();
        yield return SceneManager.Instance.InitSuccessWait();

        SceneManager.Instance.SceneChange(SceneType.Lobby);
    }
}
