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
    protected float m_fMoveSpeed;

    protected Vector3 m_vMoveTarget;

    protected bool m_bValidLife = false;

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

    public bool IsSameInteraction(InteractionData _interactionPacketData)
    {
        //Debug.Log(_interactionPacketData.m_eType.ToString() + ","
        //    + GetInteractionType.ToString() + "," 
        //    + _interactionPacketData.m_iInteractionIndex.ToString() + ","
        //    + GetInteractionIndex.ToString());
        return _interactionPacketData.m_eType == GetInteractionType &&
            _interactionPacketData.m_iInteractionIndex == GetInteractionIndex;
    }
    public virtual void Initialize(eInteractionType _eInteractionType, int _iInteractionIndex)
    {
        m_eInteractionType = _eInteractionType;
        m_iInteractionIndex = _iInteractionIndex;

        // 컴포넌트가 비어있으면 생성해준다.
        if(dicInteractionComponents.Count == 0)
            CreateComponent();

        InitComponent();
    }

    // 더이상 사용하지않아 풀로 들어갈때 함수호출
    public virtual void Clear()
    {
        foreach( var item in m_dicInteractionComponent)
        {
            item.Value.ClearComponent();
        }
    }

    public void UpdateInteraction(InteractionData _InteractionData)
    {
        transform.position = _InteractionData.m_vPos;
        m_vMoveTarget = _InteractionData.m_vPos;
        m_vMoveDir = _InteractionData.m_vDir;
        m_fMoveSpeed = _InteractionData.m_fMoveSpeed;

        m_bValidLife = _InteractionData.VaildLife;
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
        if (m_fMoveSpeed > 0)
        {
            transform.position += m_vMoveDir * (m_fMoveSpeed * Time.deltaTime);


            m_fMoveSpeed -= 1 * Time.deltaTime;
        }
    }
}
