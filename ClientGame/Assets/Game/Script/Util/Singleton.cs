using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class Singleton<T> where T : class, new()
{
    private static T m_singleton = null;

    protected enum ManagerState
    {
        InitBefore,
        InitAfter,
        ManagerState_MAX,
    }

    protected ManagerState m_eManagerState = ManagerState.InitBefore;


    public bool InitSuccess
    {
        get
        {
            return m_eManagerState == ManagerState.InitAfter;
        }
    }
    public static T Instance
    {
        get
        {
            if (m_singleton == null)
            {
                m_singleton = new T();
            }
            return m_singleton;
        }
    }

    public abstract void Init();

    public abstract void Destroy();

    public IEnumerator InitSuccessWait()
    {
        yield return new WaitUntil(() => { return InitSuccess; });
    }
}

#region MonoSingleton
public abstract class MonoSingleton<T> : MonoBehaviour where T : MonoBehaviour
{
    private static T m_singleton = null;

    protected enum ManagerState
    {
        InitBefore,
        InitAfter,
        ManagerState_MAX,
    }

    protected ManagerState m_eManagerState = ManagerState.InitBefore;


    public bool InitSuccess
    {
        get
        {
            return m_eManagerState == ManagerState.InitAfter;
        }
    }
    public static T Instance
    {
        get
        {
            if (m_singleton == null)
            {
                
                GameObject go = new GameObject(typeof(T).Name, typeof(T));
                m_singleton =  go.GetComponent<T>();
                //Debug.Log(typeof(T).Name);
                DontDestroyOnLoad(go);
            }
            return m_singleton;
        }
    }

    public abstract void Init();

    public abstract void Destroy();

    public IEnumerator InitSuccessWait()
    {
        yield return new WaitUntil(() => { return InitSuccess; });
    }
}
#endregion MonoSingleton

