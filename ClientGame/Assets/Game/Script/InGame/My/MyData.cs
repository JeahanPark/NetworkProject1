using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyData
{
    private int m_iPoint;

    public void UpdatePoint(int _iPoint)
    {
        m_iPoint = _iPoint;
    }
    public void AddPoint(int _iAddPoint)
    {
        m_iPoint += _iAddPoint;
    }

    public int GetPoint()
    {
        return m_iPoint;
    }
}
