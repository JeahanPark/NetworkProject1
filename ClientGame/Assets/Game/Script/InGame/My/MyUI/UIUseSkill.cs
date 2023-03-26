using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class UIUseSkill : MonoBehaviour
{
    private TextMeshProUGUI m_txtSkillName = null;
    private Button m_btnSkill = null;
    private Image m_imgSkill = null;


    private float m_fSkillMaxDelay = 3f;
    private float m_fSkillDelay = 0;
    private void Awake()
    {
        m_txtSkillName = transform.Find<TextMeshProUGUI>("Text (TMP)");
        m_btnSkill = GetComponent<Button>();
        m_btnSkill.onClick.AddListener(OnClickAttack);

        m_imgSkill = GetComponent<Image>();

        SygnalManager.Instance.Register(SygnalEventType.InGame_UpdateMySkill, UpdatetMySkillUI);
    }

    private void OnDestroy()
    {
        SygnalManager.Instance.Unregister(SygnalEventType.InGame_UpdateMySkill, UpdatetMySkillUI);
    }

    private void Update()
    {
        m_fSkillDelay += Time.deltaTime;

        if (m_fSkillDelay > m_fSkillMaxDelay)
        {
            // ��ų ��밡��
            if (!m_btnSkill.interactable)
            {
                // �̹��� Ȱ��ȭ
                m_imgSkill.fillAmount = 1;
                m_btnSkill.interactable = true;
            }
        }
        else
        {
            m_imgSkill.fillAmount = m_fSkillDelay == 0 ? 0 : m_fSkillDelay / m_fSkillMaxDelay;
        }
    }
    private void OnClickAttack()
    {
        if (m_fSkillDelay > m_fSkillMaxDelay)
        {
            InGameController.Instance.SendAttack();
        }
    }

    private void UpdatetMySkillUI()
    {
        switch (DataManager.Instance.CrtMySkill)
        {
            case eSkillType.FireBall:
                m_txtSkillName.text = "���̾";
                m_fSkillMaxDelay = 3;
                break;
            case eSkillType.Wall:
                m_txtSkillName.text = "�������";
                m_fSkillMaxDelay = 2;
                break;
            case eSkillType.Reflection:
                m_txtSkillName.text = "�ݻ�";
                m_fSkillMaxDelay = 3;
                break;
        }

        m_btnSkill.interactable = false;

        m_fSkillDelay = 0;
    }
}
