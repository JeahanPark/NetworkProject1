using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UserObject : InteractionObject
{
    private MeshRenderer m_meshRenderer = null;


    private static Material m_matNormalBody = null;
    private static Material m_matDamageBody = null;
    private void Awake()
    {
        m_meshRenderer = GetComponent<MeshRenderer>();

        if(m_matNormalBody == null)
        {
            EffectManager.Instance.GetEffect<Material>(EffectID.UserBody_Normal, (mat) =>
            {
                m_matNormalBody = mat;
            });

            EffectManager.Instance.GetEffect<Material>(EffectID.UserBody_Damage, (mat) =>
            {
                m_matDamageBody = mat;
            });
        }
    }

    private bool MyInteraction
    {
        get
        {
            return DataManager.Instance.SameMyInteraction(m_iInteractionIndex, m_eInteractionType);
        }
    }

    public void SetInitialUserData(string _strNickName)
    {
        UserInfoCom info = GetInteractionCom<UserInfoCom>(eInteractionCom.UserInfo);
        info.InitData(_strNickName);

        UICom ui = GetInteractionCom<UICom>(eInteractionCom.UI);
        ui.InitUI(_strNickName);
    }
    public override void Clear()
    {
        base.Clear();

        if (MyInteraction)
        {
            InGameController.Instance.SetMyInteraction(null);
        }

        ReturnBodyMaterial();
    }

    public void Die()
    {

        Clear();
    }

    public float GetMoveSpeed
    {
        get
        {
            return m_fMoveSpeed;
        }
    }

    public void SetUserMove(Vector3 _vMoveDir, float _fMoveSpeed)
    {
        m_vMoveDir += _vMoveDir;
        m_vMoveDir.Normalize();

        m_fMoveSpeed += _fMoveSpeed;
        m_fMoveSpeed = m_fMoveSpeed > 3 ? 3 : m_fMoveSpeed;
    }

    public override void UpdateInteraction(InteractionData _InteractionData, float _fUpdateLatency)
    {
        m_vMoveDir = _InteractionData.m_vDir;
        m_fMoveSpeed = _InteractionData.m_fMoveSpeed;

        m_bValidLife = _InteractionData.VaildLife;

        if(MyInteraction)
        {
            transform.position = _InteractionData.m_vPos;
        }
        else
        {
            m_bDeadRackoningMove = true;
            m_vDeadRackoningPos = _InteractionData.m_vPos + m_vMoveDir * _fUpdateLatency;
            m_vDeadRackoningDir = m_vDeadRackoningPos - transform.position;
            m_vDeadRackoningDir.Normalize();
        }
    }

    public override void RecivedDamage(float _fDamage)
    {
        base.RecivedDamage(_fDamage);


        if(m_coDamageAnim != null)
        {
            StopCoroutine(m_coDamageAnim);
            m_coDamageAnim = null;
        }
        m_coDamageAnim = StartCoroutine(CoDamageAnim());
    }

    private Coroutine m_coDamageAnim = null;
    private IEnumerator CoDamageAnim()
    {
        
        m_meshRenderer.material = m_matDamageBody;

        yield return new WaitForSeconds(0.5f);

        ReturnBodyMaterial();

        m_coDamageAnim = null;
    }

    private void ReturnBodyMaterial()
    {
        m_meshRenderer.material = m_matNormalBody;
    }

    protected override void CreateComponent()
    {
        base.CreateComponent();
        AddDicInteractionComponent(new UserInfoCom(this));
        AddDicInteractionComponent(new UICom(this));
    }

    protected override void Update()
    {
        // �̵��ؾ��Ѵ�.
        if (m_fMoveSpeed > 0)
        {
            // ���巹Ŀ�� �̵��� �켱
            if (m_bDeadRackoningMove && !MyInteraction )
            {
                // ���巹Ŀ������ ���� ��ġ�� �����δ�.
                transform.position += m_vDeadRackoningDir * m_fDeadRackoningDeltaTime;

                if ((transform.position - m_vDeadRackoningPos).magnitude < 1f)
                {
                    m_bDeadRackoningMove = false;
                }
            }
            else
            {
                transform.position += m_vMoveDir * (m_fMoveSpeed * Time.deltaTime);
                m_fMoveSpeed -= 1 * Time.deltaTime;
            }
        }
        else
            m_bDeadRackoningMove = false;
    }
}
