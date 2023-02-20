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

        // 이거는 나중에 수정함
        // 이펙트 매니저가 초기화
        {
            EffectManager.Instance.Init();

            yield return EffectManager.Instance.InitSuccessWait();
        }

        // 소켓 연결
        {
            bool bResult = false;
            ServerConnect((result) =>
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

    private void GameSetting()
    {
        Application.targetFrameRate = 60;
    }

    public void ServerConnect(System.Action<bool> _funResult)
    {
        SocketManager.Instance.ServerConnect((result) =>
        {
            _funResult(result);
        });
    }
}
