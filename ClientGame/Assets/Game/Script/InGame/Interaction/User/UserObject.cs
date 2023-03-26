using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UserObject : InteractionObject
{
    private MeshRenderer m_meshRenderer = null;


    private static Material m_matNormalBody = null;
    private static Material m_matDamageBody = null;

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

    public override void Die()
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
        // 회전은 추측항법을 적용하지않는다. 보간만 적용
        m_vRotateY = _InteractionData.m_vRotateY;
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


        // 두벡터간의 사이각
        float cosTheta = Vector3.Dot(transform.forward, m_vRotateY) / (transform.forward.magnitude * m_vRotateY.magnitude);
        float angle = Mathf.Acos(cosTheta) * Mathf.Rad2Deg;

        // 새로 기준이 될 벡터가 오른쪽이냐 왼쪽이냐
        bool bRight = false;

        if ((Vector3.Cross(transform.forward, m_vRotateY).y > 0))
            bRight = true;

        // 사이각 차이가 0.001보다 크냐?
        if (angle > 0.001f)
        {
            Debug.Log(angle);

            // 이번틱에 뺄 각도
            float calculAngle = m_fRotateSpeed * Time.deltaTime;

            // 0이하면 angle만큼 뺀다.
            calculAngle = angle - calculAngle < 0 ? angle : calculAngle;

            // 각도에 해당하는 쿼터니언을 구한다.
            Quaternion qAngle = new Quaternion();

            float radian = calculAngle * Mathf.Deg2Rad;
            qAngle.x = 0;
            qAngle.y = Mathf.Sin(radian / 2); 
            qAngle.z = 0;
            qAngle.w = Mathf.Cos(radian / 2);

            // 방향이 왼쪽일떄
            if (!bRight)
            {
                ////-각도를 더해야한다.

                //// 크기의 제곱구하기
                //float norm = Mathf.Sqrt(Mathf.Pow(qAngle.y, 2) * Mathf.Pow(qAngle.w, 2));

                //// -각도 구하기
                //Quaternion qInversAngle = qAngle;
                //qInversAngle.y = (qInversAngle.y * right) / norm;
                //qInversAngle.w = qInversAngle.w / norm;


                //qAngle = qInversAngle;

                qAngle = Quaternion.Inverse(qAngle);
            }

            transform.rotation *= qAngle;
        }


    }
}
