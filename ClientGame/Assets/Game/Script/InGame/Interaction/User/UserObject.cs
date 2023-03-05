using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UserObject : InteractionObject
{
    private MeshRenderer m_meshRenderer = null;


    private static Material m_matNormalBody = null;
    private static Material m_matDamageBody = null;

    public Vector3 m_test;
    private void Start()
    {
        m_meshRenderer = GetComponent<MeshRenderer>();

        if(m_matNormalBody == null)
        {
            InGameController.Instance.GetEffectWorker.GetEffect<Material>(EffectType.UserBody_Normal, (mat) =>
            {
                m_matNormalBody = mat;
            });

            InGameController.Instance.GetEffectWorker.GetEffect<Material>(EffectType.UserBody_Damage, (mat) =>
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

        ReturnBodyMaterial();
    }

    public override void Initialize(InteractionData _data)
    {
        base.Initialize(_data);

        InGameController.Instance.GetEffectWorker.GetPrefabEffect(EffectType.UserCreateEffect, transform.position);
    }

    public void Die()
    {
        InGameController.Instance.GetEffectWorker.GetPrefabEffect(EffectType.DieEffect, transform.position);

        if (MyInteraction)
        {
            InGameController.Instance.SetMyInteraction(null);
            PopupManager.Instance.OpenPopup(PopupID.UIUserRiseAgain);
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
        m_vMoveDir = _InteractionData.m_vMoveDir;
        m_fMoveSpeed = _InteractionData.m_fMoveSpeed;
        m_vRotateY = _InteractionData.m_vRotateY;

        m_bValidLife = _InteractionData.VaildLife;

        if (MyInteraction)
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

    public Vector3 GetRotateY
    {
        get
        {
            return m_vRotateY;
        }
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
        // 이동해야한다.
        if (m_fMoveSpeed > 0)
        {
            // 데드레커닝 이동이 우선
            if (m_bDeadRackoningMove && !MyInteraction )
            {
                // 데드레커닝으로 구한 위치로 움직인다.
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

        transform.forward = m_vRotateY;
    }
}
