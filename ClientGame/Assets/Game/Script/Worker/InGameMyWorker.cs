using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameMyWorker : MonoBehaviour
{
    [SerializeField]
    private MyCamera m_MyCamera;

    private void Start()
    {
        InGameController.Instance.SetInGameWorker(this);
    }

    public void SetTarget(Transform _target)
    {
        m_MyCamera.SetTarget(_target);
    }
}
