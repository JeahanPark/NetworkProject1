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

    private InGameMyWorker m_InGameMy = null;

    private void Awake()
    {
        m_InGameMy = GetComponent<InGameMyWorker>();
    }

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
        {
            if(m_iKeyDown == (int)KeyDown.KeyDown_NonePressing)
            {
                // �ȴ�����
                m_fKeyPressingTime = 0;
            }
            else
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
                    if(m_fKeyPressingTime <= 0.6f)
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
        Vector3 dir = Vector3.zero;

        if(IsPressingKey(m_iKeyDown, KeyDown.KeyDown_D))
        {
            dir.x = 1;
        }

        if (IsPressingKey(m_iKeyDown, KeyDown.KeyDown_A))
        {
            dir.x = -1;
        }

        if (IsPressingKey(m_iKeyDown, KeyDown.KeyDown_W))
        {
            dir.z = 1;
        }

        if (IsPressingKey(m_iKeyDown, KeyDown.KeyDown_S))
        {
            dir.z = -1;
        }

        if (m_InGameMy != null)
            m_InGameMy.UserMove(dir, 1);
    }
}