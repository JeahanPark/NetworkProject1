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
        // 이전 키 저장
        int iBeforeKeyDonw = m_iKeyDown;

        // 키 누른거 초기화
        m_iKeyDown = (int)KeyDown.KeyDown_NonePressing;

        // 이동방향키 누르는거 처리
        {
            // 1. 이동키를 눌렀다.
            // 2. 이전에 반대편키를 안누르고 있다.
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

        // 패킷 보내기 조건
        // 1. 처음 눌렀을때
        // 2. 키 누른지 10프레임 지났을때
        {
            if(m_iKeyDown == (int)KeyDown.KeyDown_NonePressing)
            {
                // 안눌렀다
                m_fKeyPressingTime = 0;
            }
            else
            {
                // 눌렀다.
                m_fKeyPressingTime += Time.deltaTime;

                if ((m_iKeyDown & iBeforeKeyDonw) != m_iKeyDown)
                {
                    // 처음 눌렀다.
                    KeyDownProgress();
                }
                else
                {
                    // 이미 눌러져있는 키들이다.

                    // 10프레임마다 보냄
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
