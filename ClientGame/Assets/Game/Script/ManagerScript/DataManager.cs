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


    public void Login()
    {
        m_UserData = new UserData();
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
    int a = 0;
}
