using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using AddreesableManager;
using System.Runtime.InteropServices;

public class IntroController : MonoBehaviour
{
    private void Start()
    {
        //Vector3 v1 = new Vector3(1, 0, 0);
        //Vector3 v2 = new Vector3(0, 0, -1);

        //float cosTheta = Vector3.Dot(v1, v2) / (v1.magnitude * v2.magnitude);

        //float angle = Mathf.Acos(cosTheta) * Mathf.Rad2Deg;

        //float halfAngle = angle * 0.5f;

        //Vector3 up = Vector3.Cross(v1, v2).normalized;
        //Quaternion rotation = Quaternion.AngleAxis(halfAngle, up);

        //Vector3 result = rotation * v1;

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
            ServerConnect((result) =>
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

    public void ServerConnect(System.Action<bool> _funResult)
    {
        SocketManager.Instance.ServerConnect((result) =>
        {
            _funResult(result);
        });
    }
}
