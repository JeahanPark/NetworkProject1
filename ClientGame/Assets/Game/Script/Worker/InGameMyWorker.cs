using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameMyWorker : MonoBehaviour
{
    [SerializeField]
    private MyCamera m_MyCamera;

    private UserObject m_MyInteraction;

    private MyController m_MyController;


    private void Awake()
    {
        m_MyController = GetComponent<MyController>();

        InGameController.Instance.SetInGameMyWorker(this);
    }

    public void SetMyInteraction(UserObject _user)
    {
        // ���� ����
        m_MyInteraction = _user;

        // ī�޶� Ÿ�� ����
        m_MyCamera.SetTarget(_user == null ? null : _user.transform);

        // ������ ����
        DataManager.Instance.SetInteractionUserData(_user.GetInteractionIndex, _user.GetInteractionType);
    }

    public void UserMove(Vector3 _vDir, float _fMoveSpeed)
    {
        if(m_MyInteraction == null)
        {
            return;
        }

        // �ִ� �̵��ӵ����� �������
        m_MyInteraction.SetUserMove(_vDir, _fMoveSpeed);

        InGameController.Instance.SendMyUserMove(_vDir, _fMoveSpeed);
    }
}
