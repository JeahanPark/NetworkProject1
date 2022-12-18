using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class FindHelper
{
    public static T Find<T>(this Transform tran, string name)
    {
        T com = tran.Find(name).GetComponent<T>();
        return com;
    }
}
