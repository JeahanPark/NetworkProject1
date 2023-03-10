using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class MyUI : MonoBehaviour
{
    private void Awake()
    {
        //Transform mobileController = transform.Find<Transform>("MobileController");
        //mobileController.gameObject.SetActive(Application.platform == RuntimePlatform.Android);

        Button _Attack = transform.Find<Button>("Attack");
        _Attack.onClick.AddListener(OnClickAttack);
    }
    private const float m_fAttackMaxDelay = 3f;
    private float m_fAttackDelay = m_fAttackMaxDelay;

    private void Update()
    {
        m_fAttackDelay += Time.deltaTime;
    }

    private void OnClickAttack()
    {
        if (m_fAttackDelay > m_fAttackMaxDelay)
        {
            InGameController.Instance.SendAttack();
        }
    }
}
