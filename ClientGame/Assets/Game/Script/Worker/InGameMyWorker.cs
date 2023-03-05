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
        // ���� ����
        m_MyInteraction = _user;

        // ī�޶� Ÿ�� ����
        m_MyCamera.SetTarget(_user == null ? null : _user.transform);

        // ������ ����
        DataManager.Instance.SetInteractionUserData(_user);
    }

    public void UserMove(Vector3 _vDir, float _fMoveSpeed)
    {
        if(m_MyInteraction == null)
        {
            m_MyCamera.AddPostion(_vDir);
            return;
        }

        // �ִ� �̵��ӵ����� �������
        m_MyInteraction.SetUserMove(_vDir, _fMoveSpeed);

        InGameController.Instance.SendMyUserMove(_vDir, _fMoveSpeed);
    }
}
