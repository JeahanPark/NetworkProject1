using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.ResourceManagement.ResourceLocations;
using AddreesableManager;
using UnityEngine.UI;
using UnityEngine.Rendering.Universal;
public enum PopupID
{
    NONE,
    UIFadeInOut,
    UIMessageBox,
    UINickNameSetting,
    UIRegister,
    UILogIn,
    UIUserRiseAgain,
    UI_MAX,
}


public class PopupManager : MonoSingleton<PopupManager>
{
    private const int m_nPopupCamearaDepth = 1;

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
    /// <param name="_fnResult"> 비동기 함수다</param>
    public void OpenPopup(PopupID _ePopupID, System.Action<UIPopup> _fnAsyncResult = null)
    {
        if (!InitSuccess)
        {
            Debug.Log("초기화가 안됐는데?");
            return;
        }
            
        UIPopup popup = null;

        // 팝업이 없다. 로드해야된다.
        if(!m_dicPopupObject.TryGetValue(_ePopupID, out popup))
        {
            // 팝업을 생성한다.
            CreatePopup(_ePopupID, _fnAsyncResult);
            return;
        }
        popup.gameObject.SetActive(true);

        _fnAsyncResult?.Invoke(popup);
    }

    public UIPopup GetOpenPopup(PopupID _ePopupID)
    {
        UIPopup popup = null;
        m_dicPopupObject.TryGetValue(_ePopupID, out popup);
        return popup;
    }

    public void ClosePopup(PopupID _ePopupID)
    {
        if (!InitSuccess)
        {
            Debug.Log("초기화가 안됐는데?");
            return;
        }

        UIPopup popup = null;

        if (m_dicPopupObject.TryGetValue(_ePopupID, out popup))
        {
            if(!popup.gameObject.activeSelf)
            {
                // 뭐지?ㅅ
                Debug.Log("이미 닫혀있는 팝업을 닫았다?");
                return;
            }
            popup.gameObject.SetActive(false);
        }
        else
        {
            Debug.Log(string.Format("{0} 팝업이 없다",_ePopupID.ToString()));
        }
    }
    private void CreatePopup(PopupID _ePopupID, System.Action<UIPopup> _fnAsyncResult)
    {
        IResourceLocation resourceLocation = m_dicResourceLocaation[_ePopupID];

        if (resourceLocation == null)
        {
            Debug.LogError("없는 UI");
            _fnAsyncResult?.Invoke(null);
            return;
        }
        if (m_goPopupParent == null)
        {
            Debug.Log("부모 팝업 오브젝트가 생성이 안됐다.");
            _fnAsyncResult?.Invoke(null);
            return;
        }

        AddressableManager.Instance.InstanceIResourceLocation(resourceLocation, m_goPopupParent.transform,(_Result) =>
        {
            if (_Result == null)
            {
                Debug.Log(string.Format("resourceLocation로 받아온 GameObject가 null , ID : {0}", _ePopupID));
                _fnAsyncResult?.Invoke(null);
                return;
            }
            UIPopup uIPopup = _Result.GetComponent<UIPopup>();
            if (uIPopup == null)
            {
                Debug.Log(string.Format("해당 팝업에 Popup컴포넌트가 붙어있지않음 , ID : {0}", _ePopupID));
                _fnAsyncResult?.Invoke(null);
                return;
            }

            m_dicPopupObject.Add(uIPopup.GetPopupID(), uIPopup);
            _fnAsyncResult?.Invoke(uIPopup);
        });
    }


    private void CanvasCreate()
    {
        // 팝업 부모 오브젝트생성
        m_goPopupParent = new GameObject("PopupParent", typeof(RectTransform));
        m_goPopupParent.transform.SetParent(this.transform);
        m_goPopupParent.layer = LayerMask.NameToLayer("UI");


        //캔버스 컴포넌트를 붙인다.
        Canvas canvas = m_goPopupParent.AddComponent<Canvas>();
        canvas.renderMode = RenderMode.ScreenSpaceCamera;
        canvas.sortingOrder = 1;

        CanvasScaler canvasScaler = m_goPopupParent.AddComponent<CanvasScaler>();
        canvasScaler.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
        canvasScaler.referenceResolution = new Vector2(1600, 900);

        m_goPopupParent.AddComponent<GraphicRaycaster>();



        // 팝업 카메라 생성
        {
            // 카메라 오브젝트 생성
            GameObject cameraObject = new GameObject("Camera", typeof(RectTransform));

            cameraObject.transform.SetParent(this.transform);

            // 카메라 생성
            Camera camera = cameraObject.AddComponent<Camera>();

            camera.clearFlags = CameraClearFlags.Depth;
            camera.depth = m_nPopupCamearaDepth;
            // 캔버스에 카메라 세팅
            canvas.worldCamera = camera;

            camera.cullingMask = LayerMask.GetMask("UI");

            var urpData = cameraObject.AddComponent<UniversalAdditionalCameraData>();
            urpData.renderType = CameraRenderType.Overlay; // 또는 Base, 용도에 맞게 설정
            urpData.renderPostProcessing = false;
        }


    }

    private IEnumerator InitProcessor()
    {
        m_eManagerState = ManagerState.InitBefore;

        CanvasCreate();

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
                    if (id.ToString() == lisResult[i].PrimaryKey && !m_dicResourceLocaation.ContainsKey(id))
                    {
                        m_dicResourceLocaation.Add(id, lisResult[i]);
                        break;
                    }
                }
            }

            // 팝업을 날린다.
            PopupClear();
        }


        m_eManagerState = ManagerState.InitSuccess;
    }

    private void PopupClear()
    {
        m_dicPopupObject.Clear();
    }
}
