using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

// �� ��ũ��Ʈ�� GameObjec���ε� RectTransform�� ������ GetComponent�� �ϰų� transform�� ����ȯ�ؼ�
// ��ߵǼ� �̷��� ����Ѵ�. �׷��� RectTrasnform�� ����ϴ� GameObject�� �̰Ÿ� ��ӹ���
public class UIMonoBehaviour : MonoBehaviour
{
    [HideInInspector]
    public RectTransform m_rectTransform = null;
    protected virtual void Awake()
    {
        m_rectTransform = GetComponent<RectTransform>();
        if(m_rectTransform == null)
        {
            Debug.Log("UIMonoBehaviour�� �ٿ��µ� RectTransform�̾���.");
        }
    }
}
