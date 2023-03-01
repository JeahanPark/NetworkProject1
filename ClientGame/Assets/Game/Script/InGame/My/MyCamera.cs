using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyCamera : MonoBehaviour
{
    private Transform m_transTarget = null;

    private Vector3 m_TargetDistance;

    private const float m_fFreeViewMoveSpeed = 2f;

    private void Awake()
    {
        m_TargetDistance = new Vector3(0, 10, -10);

    }

    public void SetTarget(Transform _target)
    {
        m_transTarget = _target;
    }
    public void AddPostion(Vector3 _vDirPos)
    {
        transform.position += _vDirPos * m_fFreeViewMoveSpeed;
    }

    public void LateUpdate()
    {
        // 타겟이 없다
        if (m_transTarget == null)
            return;

        // 위치 잡기
        transform.position = m_transTarget.position + m_TargetDistance;
    }
}
