using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameUIWorker : MonoBehaviour
{
    private UserUI m_uiUserOgirin = null;
    private Transform m_tranWorldUIParent = null;
    private Transform m_tranUnUseUIParent = null;

    private Queue<UserUI> m_poolUserUI = null;
    private Dictionary<int, UserUI> m_dicActiveUserUI = null;

    private void Awake()
    {
        InGameController.Instance.SetInGameUIWorker(this);

        m_uiUserOgirin = transform.Find<UserUI>("OriginUI/OriginUserUI");
        m_uiUserOgirin.gameObject.SetActive(false);

        m_tranWorldUIParent = transform.Find("WorldUI");
        m_tranUnUseUIParent = transform.Find("UnUseUI");
        m_tranUnUseUIParent.gameObject.SetActive(false);

        m_poolUserUI = new Queue<UserUI>();
        m_dicActiveUserUI = new Dictionary<int, UserUI>();
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
            uIUser = m_poolUserUI.Dequeue();

        

        m_dicActiveUserUI.Add(_iUserInteraction, uIUser);

        return uIUser;
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