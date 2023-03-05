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
        // 3. 마우스
        {
            if(m_iKeyDown == (int)KeyDown.KeyDown_NonePressing)
            {
                // 안눌렀다
                m_fKeyPressingTime = 0;
            }


            if(m_iKeyDown != (int)KeyDown.KeyDown_NonePressing)
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
