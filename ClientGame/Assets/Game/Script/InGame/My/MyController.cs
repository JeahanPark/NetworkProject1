using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyController : MonoBehaviour
{
    public enum KeyDown
    {
        KeyDown_NonePressing = 0,
        KeyDown_W = 1,
        KeyDown_A = 2,
        KeyDown_S = 4,
        KeyDown_D = 8
    }
    private int m_iKeyDown = 0;
    private float m_fKeyPressingTime = 0;

    private void Update()
    {
        // ���� Ű ����
        int iBeforeKeyDonw = m_iKeyDown;

        // Ű ������ �ʱ�ȭ
        m_iKeyDown = (int)KeyDown.KeyDown_NonePressing;

        // �̵�����Ű �����°� ó��
        {
            // 1. �̵�Ű�� ������.
            // 2. ������ �ݴ���Ű�� �ȴ����� �ִ�.
            if (Input.GetKey(KeyCode.W))
            {
                m_iKeyDown += (int)KeyDown.KeyDown_W;
            }
            if (Input.GetKey(KeyCode.A))
            {
                m_iKeyDown += (int)KeyDown.KeyDown_A;
            }
            if (Input.GetKey(KeyCode.S))
            {
                m_iKeyDown += (int)KeyDown.KeyDown_S;
            }
            if (Input.GetKey(KeyCode.D))
            {
                m_iKeyDown += (int)KeyDown.KeyDown_D;
            }
        }

        // ��Ŷ ������ ����
        // 1. ó�� ��������
        // 2. Ű ������ 10������ ��������
        // 3. ���콺
        {
            if(m_iKeyDown == (int)KeyDown.KeyDown_NonePressing)
            {
                // �ȴ�����
                m_fKeyPressingTime = 0;
            }


            if(m_iKeyDown != (int)KeyDown.KeyDown_NonePressing)
            {
                // ������.
                m_fKeyPressingTime += Time.deltaTime;

                if ((m_iKeyDown & iBeforeKeyDonw) != m_iKeyDown)
                {
                    // ó�� ������.
                    KeyDownProgress();
                }
                else
                {
                    // �̹� �������ִ� Ű���̴�.

                    // 10�����Ӹ��� ����
                    if(m_fKeyPressingTime > EnumType.m_f10Frame)
                    {
                        m_fKeyPressingTime = 0;
                        KeyDownProgress();
                    }
                }
            }
        }
    }

    private bool IsPressingKey(int _iKeyDown, KeyDown _ekeyDown)
    {
        int iKeyDownIndex = (int)_ekeyDown;
        return (_iKeyDown & iKeyDownIndex) == iKeyDownIndex;
    }

    private void KeyDownProgress()
    {
        Vector3 vMoveDir = Vector3.zero;
        Vector3 vRotateY = InGameController.Instance.GetMyWorker.GetMyInteraction.GetRotateY;
        if(IsPressingKey(m_iKeyDown, KeyDown.KeyDown_D))
        {
            vMoveDir.x = 1;
        }

        if (IsPressingKey(m_iKeyDown, KeyDown.KeyDown_A))
        {
            vMoveDir.x = -1;
        }

        if (IsPressingKey(m_iKeyDown, KeyDown.KeyDown_W))
        {
            vMoveDir.z = 1;
        }

        if (IsPressingKey(m_iKeyDown, KeyDown.KeyDown_S))
        {
            vMoveDir.z = -1;
        }

        

        InGameController.Instance.GetMyWorker.UserMove(vMoveDir, vRotateY, 1);           
    }
}
