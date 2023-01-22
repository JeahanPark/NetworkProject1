using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyCamera : MonoBehaviour
{
    private Transform m_transTarget = null;

    private Vector3 m_TargetDistance;

    private void Awake()
    {
        m_TargetDistance = new Vector3(0, 10, -10);

    }

    public void SetTarget(Transform _target)
    {
        m_transTarget = _target;
    }

    public void LateUpdate()
    {
        // Ÿ���� ����
        if (m_transTarget == null)
            return;

        // ��ġ ���
        transform.position = m_transTarget.position + m_TargetDistance;
    }
}