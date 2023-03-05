using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIMobileJoyStick : MonoBehaviour
{
    private FixedJoystick m_joyStickLeft = null;
    private FixedJoystick m_joyStickRight = null;

    private float m_fKeyPressingTime = 0;

    private void Awake()
    {
        m_joyStickLeft = transform.Find<FixedJoystick>("Left");
        m_joyStickRight = transform.Find<FixedJoystick>("Right");
    }

    private void Update()
    {
        float fMoveX = m_joyStickLeft.Horizontal;
        float fMoveY = m_joyStickLeft.Vertical;

        float fRotateX = m_joyStickRight.Horizontal;
        float fRotateY = m_joyStickRight.Vertical;

        if (fMoveX != 0 || fMoveY != 0 || fRotateX != 0 || fRotateY != 0)
        {
            m_fKeyPressingTime += Time.deltaTime;

            if(m_fKeyPressingTime > EnumType.m_f10Frame)
            {
                m_fKeyPressingTime = 0;

                Vector3 vMoveDir = new Vector3(fMoveX, 0, fMoveY);
                Vector3 vRotateDir = new Vector3(fRotateX, 0, fRotateY);
                float fSpeed = 1;
                if (vMoveDir == Vector3.zero)
                {
                    fSpeed = 0;
                    vMoveDir = InGameController.Instance.GetMyWorker.GetMyInteraction.GetMoveDir;
                }

                if(vRotateDir == Vector3.zero)
                {
                    vRotateDir = InGameController.Instance.GetMyWorker.GetMyInteraction.GetRotateY;
                }
                InGameController.Instance.GetMyWorker.UserMove(vMoveDir, vRotateDir, fSpeed);
            }
        }
    }
}
