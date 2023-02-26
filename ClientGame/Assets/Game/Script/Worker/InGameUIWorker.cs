using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameUIWorker : MonoBehaviour
{
    private UserUI m_uiUserOgirin = null;
    private DamageUI m_uiDamageOgirin = null;
    private Transform m_tranWorldUIParent = null;
    private Transform m_tranUnUseUIParent = null;

    // 만약 3개가 되면 하나로 합치자
    private Queue<UserUI> m_poolUserUI = null;
    private Queue<DamageUI> m_poolDamageUI = null;
    private Dictionary<int, UserUI> m_dicActiveUserUI = null;

    [SerializeField]
    private bool m_bTestFun = false;
    private Canvas m_canvas = null;
    public Canvas GetCanvas
    {
        get
        {
            return m_canvas;
        }
    }

    private void Update()
    {
        if(m_bTestFun)
        {
            m_bTestFun = false;
            TestFun();
        }
    }

    private void TestFun()
    {
        ShowDamageUI(9999, Vector3.zero);
    }

    private void Awake()
    {
        InGameController.Instance.SetInGameUIWorker(this);

        m_uiUserOgirin = transform.Find<UserUI>("OriginUI/OriginUserUI");
        m_uiUserOgirin.gameObject.SetActive(false);

        m_uiDamageOgirin = transform.Find<DamageUI>("OriginUI/OriginDamageUI");
        m_uiDamageOgirin.gameObject.SetActive(false);

        m_tranWorldUIParent = transform.Find("WorldUI");
        m_tranUnUseUIParent = transform.Find("UnUseUI");
        m_tranUnUseUIParent.gameObject.SetActive(false);

        m_poolUserUI = new Queue<UserUI>();
        m_poolDamageUI = new Queue<DamageUI>();
        m_dicActiveUserUI = new Dictionary<int, UserUI>();

        m_canvas = GetComponent<Canvas>();
    }

    public UserUI CreateUIUser(int _iUserInteraction)
    {
        UserUI uIUser = null;

        if (m_poolUserUI.Count == 0)
        {
            uIUser = Instantiate<UserUI>(m_uiUserOgirin, m_tranWorldUIParent);
            uIUser.gameObject.SetActive(true);
        }
        else
        {
            uIUser = m_poolUserUI.Dequeue();
            uIUser.transform.SetParent(m_tranWorldUIParent, false);
        }
        m_dicActiveUserUI.Add(_iUserInteraction, uIUser);

        return uIUser;
    }
    public DamageUI CreatDamageUI()
    {
        DamageUI ui = null;

        if (m_poolDamageUI.Count == 0)
        {
            ui = Instantiate<DamageUI>(m_uiDamageOgirin, m_tranWorldUIParent);
            ui.gameObject.SetActive(true);
        }
        else
        {
            ui = m_poolDamageUI.Dequeue();
            ui.transform.SetParent(m_tranWorldUIParent, false);
        }
            

        return ui;
    }
    public void ShowDamageUI(float _fDamage, Vector3 _vPos)
    {
        DamageUI ui = CreatDamageUI();
        ui.InitUI(_fDamage, _vPos);
    }

    public void HideDamamgeUI(DamageUI damageUI)
    {
        m_poolDamageUI.Enqueue(damageUI);
        damageUI.transform.SetParent(m_tranUnUseUIParent, false);
    }

    public void PopUIUser(int _iUserInteraction)
    {
        UserUI ui = null;
        if(m_dicActiveUserUI.TryGetValue(_iUserInteraction, out ui))
        {
            m_dicActiveUserUI.Remove(_iUserInteraction);
            m_poolUserUI.Enqueue(ui);
            ui.transform.SetParent(m_tranUnUseUIParent, false);
        }
    }
}