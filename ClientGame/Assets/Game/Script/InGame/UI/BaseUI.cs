using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BaseUI : MonoBehaviour
{
    protected RectTransform m_rectTransform = null;

    protected virtual void Awake()
    {
        m_rectTransform = GetComponent<RectTransform>();
    }
    public virtual void ClearData()
    {

    }

    protected Vector3 ChangePosWorldToUI(Vector3 _vWorldPos)
    {
        Vector3 dir = (_vWorldPos - Camera.main.transform.position).normalized;

        return Camera.main.transform.position + dir * InGameController.Instance.GetInGameUIWorker.GetCanvas.planeDistance;
    }
}
