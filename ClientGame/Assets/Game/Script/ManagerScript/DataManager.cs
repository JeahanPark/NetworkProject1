using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DataManager : Singleton<DataManager>
{
    private UserData m_UserData = null;
    public bool IsLogin()
    {
        return m_UserData != null;
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
