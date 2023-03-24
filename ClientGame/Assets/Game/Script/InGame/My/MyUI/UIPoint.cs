using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
public class UIPoint : MonoBehaviour
{
    private long m_lBeforePoint = 0;
    private TextMeshProUGUI m_txtPoint = null;
    private TextMeshProUGUI m_txtAddPoint = null;

    private float m_fAddPointEffectCrtTime = 0f;
    private float m_fAddPointEffectMaxTime = 3f;
    private const float m_fMoveSpeed = 30;

    private void Awake()
    {
        m_txtPoint = GetComponent<TextMeshProUGUI>();
        m_txtAddPoint = transform.Find<TextMeshProUGUI>("AddPoint");

        SygnalManager.Instance.Register(SygnalEventType.InGame_UpdatePoint, UpdatetMyPoint);
    }

    private void Start()
    {
        RefreshUI();
    }

    private void OnDestroy()
    {
        SygnalManager.Instance.Unregister(SygnalEventType.InGame_UpdatePoint, UpdatetMyPoint);
    }

    private void UpdatetMyPoint()
    {
        RefreshUI();
        StartAddPointEffect();
    }

    private void RefreshUI()
    {
        long crtPoint = InGameController.Instance.GetMyWorker.GetMyData.GetPoint();
        m_txtPoint.text = string.Format("Á¡¼ö : {0}", crtPoint);

        long addPoint = crtPoint - m_lBeforePoint;
        m_txtAddPoint.text = string.Format("+{0}", addPoint);

        m_txtAddPoint.gameObject.SetActive(false);

        m_lBeforePoint = crtPoint;
    }

    private void Update()
    {
        if(m_txtAddPoint.gameObject.activeSelf)
        {
            m_fAddPointEffectCrtTime += Time.deltaTime;

            Vector2 pos = m_txtAddPoint.rectTransform.anchoredPosition;
            pos.y += Time.deltaTime * m_fMoveSpeed;

            m_txtAddPoint.rectTransform.anchoredPosition = pos;

            if (m_fAddPointEffectCrtTime >= m_fAddPointEffectMaxTime)
            {
                EndAddPointEffect();
            }
        }
    }

    private void StartAddPointEffect()
    {
        m_fAddPointEffectCrtTime = 0;
        m_txtAddPoint.gameObject.SetActive(false);
        m_txtAddPoint.rectTransform.anchoredPosition = Vector2.zero;
    }

    private void EndAddPointEffect()
    {
        m_txtAddPoint.gameObject.SetActive(true);
        m_fAddPointEffectCrtTime = 0;
    }
}
