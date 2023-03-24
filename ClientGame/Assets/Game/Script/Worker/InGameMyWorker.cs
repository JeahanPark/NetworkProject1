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

    private MyData m_MyData = new MyData();

    public UserObject GetMyInteraction
    {
        get
        {
            return m_MyInteraction;
        }
    }

    public bool IsNullMyInteraction
    {
        get
        {
            return m_MyInteraction == null;
        }
    }

    public MyData GetMyData
    {
        get
        {
            return m_MyData;
        }
    }

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

        if (_user != null)
        {
            // ��ų UI Update
            SygnalManager.Instance.Fire(SygnalEventType.InGame_UpdateMySkill);
        }
    }

    public void UserMove(Vector3 _vMoveDir, Vector3 _vRotateY, float _fMoveSpeed)
    {
        if(m_MyInteraction == null)
        {
            m_MyCamera.AddPostion(_vMoveDir);
            return;
        }

        // �ִ� �̵��ӵ����� �������
        m_MyInteraction.SetUserMove(_vMoveDir, _fMoveSpeed);

        InGameController.Instance.SendMyUserMove(_vMoveDir, _vRotateY, _fMoveSpeed);
    }

    private void UpdateAttackUI()
    {

    }
}
