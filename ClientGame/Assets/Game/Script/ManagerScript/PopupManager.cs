using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.ResourceManagement.ResourceLocations;
using AddreesableManager;
using UnityEngine.UI;
public enum PopupID
{
    UIFadeInOut,
    UIMessageBox,
    UI_MAX,
}


public class PopupManager : Singleton<PopupManager>
{
    private Dictionary<PopupID, IResourceLocation> m_dicResourceLocaation = null;
    private Dictionary<PopupID, UIPopup> m_dicPopupObject = new Dictionary<PopupID, UIPopup>();
    private GameObject m_goPopupParent = null;
    public override void Destroy()
    {
    }

    public override void Init()
    {
        StartCoroutine(InitProcessor());
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="_ePopupID"></param>
    /// <param name="_fnResult"> �񵿱� �Լ���</param>
    public void PopupOpen(PopupID _ePopupID, System.Action<UIPopup> _fnAsyncResult)
    {
        if (!InitSuccess)
            return;
        UIPopup popup = null;

        // �˾��� ����. �ε��ؾߵȴ�.
        if(!m_dicPopupObject.TryGetValue(_ePopupID, out popup))
        {
            // �˾��� �����Ѵ�.
            CreatePopup(_ePopupID, _fnAsyncResult);
            return;
        }
        _fnAsyncResult(popup);
    }
    private void CreatePopup(PopupID _ePopupID, System.Action<UIPopup> _fnAsyncResult)
    {
        IResourceLocation resourceLocation = m_dicResourceLocaation[_ePopupID];

        if (resourceLocation == null)
        {
            Debug.LogError("���� UI");
            _fnAsyncResult(null);
            return;
        }
        if (m_goPopupParent == null)
        {
            Debug.Log("�θ� �˾� ������Ʈ�� ������ �ȵƴ�.");
            _fnAsyncResult(null);
            return;
        }

        AddressableManager.Instance.InstanceIResourceLocation(resourceLocation, m_goPopupParent.transform,(_Result) =>
        {
            if (_Result == null)
            {
                Debug.Log(string.Format("resourceLocation�� �޾ƿ� GameObject�� null , ID : {0}", _ePopupID));
                _fnAsyncResult(null);
                return;
            }
            UIPopup uIPopup = _Result.GetComponent<UIPopup>();
            if (uIPopup == null)
            {
                Debug.Log(string.Format("�ش� �˾��� Popup������Ʈ�� �پ��������� , ID : {0}", _ePopupID));
                _fnAsyncResult(null);
                return;
            }

            m_dicPopupObject.Add(uIPopup.GetPopupID(), uIPopup);
            _fnAsyncResult(uIPopup);
        });
    }


    private void CanvasCreat()
    {
        // �˾� �θ� ������Ʈ����
        m_goPopupParent = new GameObject("PopupParent", typeof(RectTransform));
        m_goPopupParent.transform.SetParent(this.transform);

        //ĵ���� ������Ʈ�� ���δ�.
        Canvas canvas = m_goPopupParent.AddComponent<Canvas>();
        canvas.renderMode = RenderMode.ScreenSpaceCamera;

        m_goPopupParent.AddComponent<CanvasScaler>();
        m_goPopupParent.AddComponent<GraphicRaycaster>();



        // �˾� ī�޶� ����
        {
            // ī�޶� ������Ʈ ����
            GameObject cameraObject = new GameObject("Camera", typeof(RectTransform));

            cameraObject.transform.SetParent(this.transform);

            // ī�޶� ����
            Camera camera = cameraObject.AddComponent<Camera>();

            // ĵ������ ī�޶� ����
            canvas.worldCamera = camera;
        }


    }

    private IEnumerator InitProcessor()
    {
        m_eManagerState = ManagerState.InitBefore;

        CanvasCreat();

        IList<IResourceLocation> lisResult = null;

        bool bResult = false;

        AddressableManager.Instance.AddressableResourceLoad(AddressableLabelName.Popup, (_lisResult) =>
        {
            lisResult = _lisResult;
            bResult = true;
        });

        yield return new WaitUntil(() => { return bResult; });

        if(lisResult == null || lisResult.Count == 0)
        {
            Debug.LogWarning("Popup IResourceLocation Null");
        }
        else
        {
            m_dicResourceLocaation = new Dictionary<PopupID, IResourceLocation>();
            for (int i = 0; i < lisResult.Count; ++i)
            {
                foreach (PopupID id in System.Enum.GetValues(typeof(PopupID)))
                {
                    if (id.ToString() == lisResult[i].PrimaryKey)
                    {
                        m_dicResourceLocaation.Add(id, lisResult[i]);
                        break;
                    }
                }
            }

            // �˾��� ������.
            PopupClear();
        }


        m_eManagerState = ManagerState.InitAfter;
    }

    private void PopupClear()
    {
        m_dicPopupObject.Clear();
    }
}
