using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameMyWorker : MonoBehaviour
{
    [SerializeField]
    private MyCamera m_MyCamera;

    private UserObject m_MyInteraction;

    private MyController m_MyController;

    private MyUI m_MyUI;

    private void Awake()
    {
        m_MyController = GetComponent<MyController>();

        m_MyUI = transform.Find<MyUI>("MyUI");

        InGameController.Instance.SetInGameMyWorker(this);
    }

    public void SetMyInteraction(UserObject _user)
    {
        // 유저 세팅
        m_MyInteraction = _user;

        // 카메라 타겟 세팅
        m_MyCamera.SetTarget(_user == null ? null : _user.transform);

        // 데이터 세팅
        DataManager.Instance.SetInteractionUserData(_user);
    }

    public void UserMove(Vector3 _vDir, float _fMoveSpeed)
    {
        if(m_MyInteraction == null)
        {
            m_MyCamera.AddPostion(_vDir);
            return;
        }

        // 최대 이동속도보다 낮을경우
        m_MyInteraction.SetUserMove(_vDir, _fMoveSpeed);

        InGameController.Instance.SendMyUserMove(_vDir, _fMoveSpeed);
    }
}
