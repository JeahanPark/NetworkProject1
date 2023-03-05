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
        float x = m_joyStickLeft.Horizontal;
        float z = m_joyStickLeft.Vertical;
        if (x != 0 || z != 0)
        {
            m_fKeyPressingTime += Time.deltaTime;

            if(m_fKeyPressingTime > EnumType.m_f10Frame)
            {
                m_fKeyPressingTime = 0;

                Vector3 dir = new Vector3(x, 0, z);

                InGameController.Instance.GetMyWorker.UserMove(dir, 1);
            }
        }
    }
}
