using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestCode : MonoBehaviour
{
    public bool check = false;

#if UNITY_EDITOR
    public void Awake()
    {
        SygnalManager.Instance.Register(SygnalEventType.TEST, Fire1);
        SygnalManager.Instance.Register(SygnalEventType.TEST, Fire1);
    }
    public void OnDestory()
    {
        SygnalManager.Instance.Unregister(SygnalEventType.TEST, Fire1);
    }

    private void Update()
    {
        if(check)
        {
            check = false;
            SygnalManager.Instance.Fire(SygnalEventType.TEST);
        }
    }

    public void Fire1()
    {
        Debug.Log(gameObject.name);
    }
#endif
}
