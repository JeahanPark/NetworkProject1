using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DataManager : Singleton<DataManager>
{
    private UserData m_UserData = null;

    // 인게임에서 사용하는 데이터
    private int m_iInteractionIndex = EnumType.InteractionIndexNone;
    private eInteractionType m_eType;


    public bool IsLogin()
    {
        return m_UserData != null;
    }
    public void SetInteractionUserData(UserObject _user)
    {
        if(_user == null)
        {
            m_iInteractionIndex = EnumType.InteractionIndexNone;
            m_eType = eInteractionType.None;
        }
        else
        {
            m_iInteractionIndex = _user.GetInteractionIndex;
            m_eType = _user.GetInteractionType;
        }
    }

    public bool SameMyInteraction(int _iInteractionIndex, eInteractionType _eType)
    {
        return m_iInteractionIndex == _iInteractionIndex && _eType == m_eType;
    }

    public void SetUserData(LoginResultPacket _loginResultPacket)
    {
        m_UserData = new UserData();
        m_UserData.m_UserID = _loginResultPacket.m_UserID;
        m_UserData.m_iScore = _loginResultPacket.Score;
    }

    public void LogOut()
    {
        m_UserData = null;
    }

    public override void Destroy()
    {
    }

    public override void Init()
    {
    }
}

public class UserData
{
    public string m_UserID;
    public int m_iScore;
}
