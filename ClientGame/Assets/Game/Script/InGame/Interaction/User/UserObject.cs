using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UserObject : InteractionObject
{
    private bool MyInteraction
    {
        get
        {
            return DataManager.Instance.SameMyInteraction(m_iInteractionIndex, m_eInteractionType);
        }
    }

    public override void Initialize(eInteractionType _eInteractionType, int _iInteractionIndex)
    {
        base.Initialize(_eInteractionType, _iInteractionIndex);

        if(MyInteraction)
        {
            InGameController.Instance.SetMyInteraction(this);
        }
    }

    public void SetInitialUserData(string _strNickName)
    {
        UserInfoCom info = GetInteractionCom<UserInfoCom>(eInteractionCom.UserInfo);
        info.InitData(_strNickName);

        UICom ui = GetInteractionCom<UICom>(eInteractionCom.UI);
        ui.InitUI(_strNickName);
    }
    public override void Clear()
    {
        base.Clear();

        if (MyInteraction)
        {
            InGameController.Instance.SetMyInteraction(null);
        }
    }

    public float GetMoveSpeed
    {
        get
        {
            return m_fMoveSpeed;
        }
    }

    public void SetUserMove(Vector3 _vMoveDir, float _fMoveSpeed)
    {
        m_vMoveDir += _vMoveDir;
        m_vMoveDir.Normalize();

        m_fMoveSpeed += _fMoveSpeed;
        m_fMoveSpeed = m_fMoveSpeed > 3 ? 3 : m_fMoveSpeed;
    }

    protected override void CreateComponent()
    {
        base.CreateComponent();
        AddDicInteractionComponent(new UserInfoCom(this));
        AddDicInteractionComponent(new UICom(this));
    }

    protected override void Update()
    {
        base.Update();

    }
}
