using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cube : MonoBehaviour
{
    public float angle = 0;

    float q = 0;
    // Update is called once per frame
    void Update()
    {
        q += Time.deltaTime;

        //Quaternion q1 = AngleToQuaternion(1);
        //Quaternion q2 = AngleToQuaternion(359);

        //float qqq = Mathf.Abs(Mathf.Sin(q));
        //Debug.Log(qqq);
        //transform.rotation = CustomLerp(q1, q2, qqq);

        transform.rotation = AngleToQuaternion(angle);
    }
    private Quaternion AngleToQuaternion(float Angle)
    {
        Quaternion quaternion = new Quaternion();

        float radian = Angle * Mathf.Deg2Rad;

        quaternion.x = 0;
        quaternion.y = Mathf.Sin(radian / 2);
        quaternion.z = 0;
        quaternion.w = Mathf.Cos(radian / 2);

        transform.rotation = quaternion;

        return quaternion;
    }


    private Quaternion CustomLerp(Quaternion q1, Quaternion q2, float ratio)
    {
        float temp1 = (1f - ratio);

        q1.x *= temp1;
        q1.y *= temp1;
        q1.z *= temp1;
        q1.w *= temp1;

        q2.x *= ratio;
        q2.y *= ratio;
        q2.z *= ratio;
        q2.w *= ratio;

        Quaternion q = new Quaternion();
        q.x = q1.x + q2.x;
        q.y = q1.y + q2.y;
        q.z = q1.z + q2.z;
        q.w = q1.w + q2.w;

        return q;
    }
}
