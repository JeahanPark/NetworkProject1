using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyData
{
    private long m_lPoint;

    public void AddPoint(long _lAddPoint)
    {
        m_lPoint += _lAddPoint;
    }

    public long GetPoint()
    {
        return m_lPoint;
    }
}
