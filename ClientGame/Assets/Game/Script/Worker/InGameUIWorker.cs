using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameUIWorker : MonoBehaviour
{
    private UIUser m_uiUserOgirin = null;
    private RectTransform m_tranParent = null;

    private Queue<UIUser> m_poolUserUI = null;
    private Dictionary<int, UIUser> m_dicActiveUserUI = null;

    private void Awake()
    {
        InGameController.Instance.SetInGameUIWorker(this);

        m_uiUserOgirin = transform.Find<UIUser>("Origin/OriginUIUser");

        m_tranParent = transform as RectTransform;

        m_poolUserUI = new Queue<UIUser>();
        m_dicActiveUserUI = new Dictionary<int, UIUser>();
    }

    private void UIUserAdd(UserObject _userObject)
    {
        UIUser uIUser = null;

        if (m_poolUserUI.Count == 0)
            uIUser = Instantiate<UIUser>(m_uiUserOgirin, m_tranParent);
        else
            uIUser = m_poolUserUI.Dequeue();

        m_dicActiveUserUI.Add(_userObject.GetInteractionIndex, uIUser);
    }
}