using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class DamageUI : BaseUI
{
    private CanvasGroup m_canvasGroup = null;
    private TextMeshProUGUI m_txtDamage = null;

    private float m_fShowCrtTime = 0;
    private const float m_fShowMaxTime = 2f;

    private float m_fMoveSpeed = 35f;

    // �����ִ� �ð��� 0~1������������
    private float m_fFadeStartTimeRatio = 0.2f;
    private float m_fFadeEndTimeRatio = 1;
    private float m_fFadeDecreaseValue = 0;

    private Vector3 m_vWolrodPos;
    public void InitUI(float _fDamage, Vector3 _vPos)
    {
        m_txtDamage.text = string.Format("-{0}", _fDamage);
        m_fShowCrtTime = 0;
        m_canvasGroup.alpha = 1;

        m_vWolrodPos = _vPos;
        transform.position = ChangePosWorldToUI(m_vWolrodPos);

        m_fFadeDecreaseValue = 1 / (m_fFadeEndTimeRatio - m_fFadeStartTimeRatio) * m_fShowMaxTime;
    }
    protected override void Awake()
    {
        base.Awake();
        m_txtDamage = transform.Find<TextMeshProUGUI>("text");
        m_canvasGroup = GetComponent<CanvasGroup>();
    }

    private void Update()
    {
        m_fShowCrtTime += Time.deltaTime; 
        if(m_fShowCrtTime < m_fShowMaxTime)
        {
            // ��ġ �̵�
            {
                transform.position = ChangePosWorldToUI(m_vWolrodPos);

                Vector2 pos = m_rectTransform.anchoredPosition;
                pos.y += m_fShowCrtTime * m_fMoveSpeed;

                m_rectTransform.anchoredPosition = pos;
            }

            // ������ �縮���� ȿ��
            {
                if (m_fShowMaxTime * m_fFadeStartTimeRatio < m_fShowCrtTime && m_canvasGroup.alpha > 0)
                {
                    m_canvasGroup.alpha -= m_fFadeDecreaseValue * Time.deltaTime;
                }
            }
        }
        else
        {
            // ������� ��Ȱ��ȭ�Ǽ� Update�� ���Ѵ�.
            InGameController.Instance.GetInGameUIWorker.HideDamamgeUI(this);
        }
    }
}
