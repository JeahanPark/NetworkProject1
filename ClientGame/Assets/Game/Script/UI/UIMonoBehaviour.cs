using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

// 이 스크립트는 GameObjec는인데 RectTransform을 쓸려면 GetComponent를 하거나 transform을 형변환해서
// 써야되서 이렇게 써야한다. 그래서 RectTrasnform을 사용하는 GameObject는 이거를 상속받자
public class UIMonoBehaviour : MonoBehaviour
{
    [HideInInspector]
    public RectTransform m_rectTransform = null;
    protected virtual void Awake()
    {
        m_rectTransform = GetComponent<RectTransform>();
        if(m_rectTransform == null)
        {
            Debug.Log("UIMonoBehaviour를 붙였는데 RectTransform이없다.");
        }
    }
}
