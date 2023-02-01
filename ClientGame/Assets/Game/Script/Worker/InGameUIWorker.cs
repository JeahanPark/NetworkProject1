using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameUIWorker : MonoBehaviour
{
    private UserUI m_uiUserOgirin = null;
    private RectTransform m_tranWorldUIParent = null;

    private Queue<UserUI> m_poolUserUI = null;
    private Dictionary<int, UserUI> m_dicActiveUserUI = null;

    private void Awake()
    {
        InGameController.Instance.SetInGameUIWorker(this);

        m_uiUserOgirin = transform.Find<UserUI>("OriginUI/OriginUserUI");

        m_tranWorldUIParent = transform as RectTransform;

        m_poolUserUI = new Queue<UserUI>();
        m_dicActiveUserUI = new Dictionary<int, UserUI>();
    }

    public UserUI CreateUIUser(InteractionObject _userObject)
    {
        UserUI uIUser = null;

        if (m_poolUserUI.Count == 0)
        {
            uIUser = Instantiate<UserUI>(m_uiUserOgirin, m_tranWorldUIParent);
            uIUser.gameObject.SetActive(true);
        }
        else
            uIUser = m_poolUserUI.Dequeue();

        

        m_dicActiveUserUI.Add(_userObject.GetInteractionIndex, uIUser);

        return uIUser;
    }
}