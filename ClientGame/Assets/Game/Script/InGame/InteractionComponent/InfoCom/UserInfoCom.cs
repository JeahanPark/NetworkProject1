using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UserInfoCom : InfoCom
{
    public UserInfoCom(InteractionObject _ownerInteraction) : base(_ownerInteraction)
    {
        m_eInteractionCom = eInteractionCom.UserInfo;
    }
    protected float m_fHP = 0;
    protected string m_strNickName = string.Empty;

    public void InitData(string _nickName)
    {
        m_strNickName = _nickName;
    }
}
