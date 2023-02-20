using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
public class ReceiveBuffer
{
    public ReceiveBuffer()
    {
        m_recvBuffer = new byte[EnumType.BUFF_MAX_SIZE];
    }

    public byte[] PopPacketByte(int _iPacketSize)
    {
        // µ•¿Ã≈Õ∏¶ ≤®≥Ÿ¥Ÿ.
        byte[] data = new byte[_iPacketSize];
        Buffer.BlockCopy(m_recvBuffer, 0, data, 0, _iPacketSize);

        // µ•¿Ã≈Õ∏¶ ≤®≥Ω∏∏≈≠ æ¯æÿ¥Ÿ.
        m_iReadPos -= _iPacketSize;
        Buffer.BlockCopy(m_recvBuffer, _iPacketSize, m_recvBuffer, 0, m_iReadPos);

        return data;
    }


    public byte[] m_recvBuffer;
    public int m_iReadPos = 0;
}
