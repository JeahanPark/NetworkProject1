using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InteractionObject : MonoBehaviour
{
    public InteractionObject()
    {

    }

    protected Dictionary<eInteractionCom, BaseInteractionComponent> m_dicInteractionComponent = new Dictionary<eInteractionCom, BaseInteractionComponent>();

    public Dictionary<eInteractionCom, BaseInteractionComponent> dicInteractionComponents
    {
        get
        {
            return m_dicInteractionComponent;
        }
    }

    protected eInteractionType m_eInteractionType = eInteractionType.None;
    protected int m_iInteractionIndex = EnumType.InteractionIndexNone;

    protected Vector3 m_vMoveDir;
    protected Vector3 m_vRotateY;

    protected float m_fMoveSpeed;
    protected const float m_fRotateSpeed = 1000;
    protected bool m_bValidLife = false;

    protected bool m_bDeadRackoningMove = false;
    protected Vector3 m_vDeadRackoningPos;
    protected Vector3 m_vDeadRackoningDir;
    protected const float m_fDeadRackoningDeltaTime = 0.1f;
    public T GetInteractionCom<T>(eInteractionCom _eInteractionCom) where T : BaseInteractionComponent
    {
        BaseInteractionComponent baseInteractionComponent = null;
        if (dicInteractionComponents.TryGetValue(_eInteractionCom, out baseInteractionComponent))
            return baseInteractionComponent as T;

        return null;
    }
    public eInteractionType GetInteractionType
    {
        get { return m_eInteractionType; }
    }

    public bool GetValidLife
    {
        get
        {
            return m_bValidLife;
        }
    }

    public int GetInteractionIndex
    {
        get
        {
            return m_iInteractionIndex;
        }
    }
    public float GetMoveSpeed
    {
        get
        {
            return m_fMoveSpeed;
        }
    }
    public Vector3 GetPostion
    {
        get
        {
            return transform.position;
        }
    }
    public Vector3 GetMoveDir
    { 
        get
        {
            return m_vMoveDir;
        }
    }
    public bool IsSameInteraction(int _iInteractionIndex)
    {
        //Debug.Log(_interactionPacketData.m_eType.ToString() + ","
        //    + GetInteractionType.ToString() + "," 
        //    + _interactionPacketData.m_iInteractionIndex.ToString() + ","
        //    + GetInteractionIndex.ToString());
        return _iInteractionIndex == GetInteractionIndex;
    }
    public virtual void Initialize(InteractionData _data)
    {
        m_bDeadRackoningMove = false;

        m_eInteractionType = _data.m_eType;
        m_iInteractionIndex = _data.m_iInteractionIndex;

        // 컴포넌트가 비어있으면 생성해준다.
        if(dicInteractionComponents.Count == 0)
            CreateComponent();

        InitComponent();
        InitInteraction(_data);
    }

    private void InitInteraction(InteractionData _data)
    {
        transform.position = _data.m_vPos;
        m_vMoveDir = _data.m_vMoveDir;
        m_fMoveSpeed = _data.m_fMoveSpeed;
        m_bValidLife = _data.VaildLife;
    }

    // 더이상 사용하지않아 풀로 들어갈때 함수호출 그리고 데이터들을 정리한다.
    public virtual void Clear()
    {
        // 클리어를 할때 풀에 다시 반환해주는데 이때 m_iInteractionIndex를 사용한다.
        // 다 컴포넌트를 클리어해주고 초기화해주자
        foreach ( var item in m_dicInteractionComponent)
        {
            item.Value.ClearComponent();
        }

        m_eInteractionType = eInteractionType.None;
        m_iInteractionIndex = EnumType.InteractionIndexNone;
    }

    public virtual void UpdateInteraction(InteractionData _InteractionData, float _fUpdateLatency)
    {
        m_vMoveDir = _InteractionData.m_vMoveDir;
        m_fMoveSpeed = _InteractionData.m_fMoveSpeed;
        m_vRotateY = _InteractionData.m_vRotateY;

        if (m_fMoveSpeed <= 0)
        {
            // 이동중이지 않는다면 위치를 그냥박자
            transform.position = _InteractionData.m_vPos;
        }

        m_bValidLife = _InteractionData.VaildLife;

        m_bDeadRackoningMove = true;
        m_vDeadRackoningPos = _InteractionData.m_vPos + m_vMoveDir * _fUpdateLatency;
        m_vDeadRackoningDir = m_vDeadRackoningPos - transform.position;
        m_vDeadRackoningDir.Normalize();
    }

    public virtual void RecivedDamage(float _fDamage)
    {
        InGameController.Instance.GetUIWorker.ShowDamageUI(_fDamage, transform.position);
    }

    // 죽고나서 처리해야될것들 Clear전에 호출한다.
    public virtual void Die()
    {

    }
    protected virtual void CreateComponent()
    {
    }

    private void InitComponent()
    {
        foreach (var data in m_dicInteractionComponent)
        {
            data.Value.InitComponent();
        }
    }

    protected void AddDicInteractionComponent(BaseInteractionComponent _baseInteractionComponent)
    {
        m_dicInteractionComponent.Add(_baseInteractionComponent.InteractionComType, _baseInteractionComponent);
    }

    

    protected virtual void Update()
    {
        // 이동해야한다.
        if (m_fMoveSpeed > 0)
        {
            // 데드레커닝 이동이 우선
            if (m_bDeadRackoningMove)
            {
                // 데드레커닝으로 구한 위치로 0.1초안에 움직인다.
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
