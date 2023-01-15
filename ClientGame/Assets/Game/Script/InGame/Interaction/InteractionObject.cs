using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InteractionObject : MonoBehaviour
{
    public InteractionObject()
    {

    }

    protected List<BaseInteractionComponent> m_lisInteractionComponent = new List<BaseInteractionComponent>();

    protected eInteractionType m_eInteractionType = eInteractionType.None;
    protected int m_iInteractionIndex = EnumType.InteractionIndexNone;

    protected Vector3 m_vMoveDir;
    protected float m_fMoveSpeed;

    protected Vector3 m_vMoveTarget;

    public eInteractionType GetInteractionType
    {
        get { return m_eInteractionType; }
    }

    public int GetInteractionIndex
    {
        get
        {
            return m_iInteractionIndex;
        }
    }

    protected virtual void CraeteComponent()
    {
        m_lisInteractionComponent.Add(new InfoCom());
    }

    public bool IsSameInteraction(InteractionPacketData _interactionPacketData)
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
    }

    // 더이상 사용하지않아 풀로 들어갈때 함수호출
    public virtual void Clear()
    {

    }

    public void UpdateInteraction(InteractionPacketData _InteractionData)
    {
        transform.position = _InteractionData.m_vPos;
        m_vMoveTarget = _InteractionData.m_vPos;
        m_vMoveDir = _InteractionData.m_vDir;
        m_fMoveSpeed = _InteractionData.m_fMoveSpeed;
    }



    public bool IsDelete()
    {
        return false;
    }

    

    private void Update()
    {
        if (m_fMoveSpeed > 0)
        {
            transform.position += m_vMoveDir * (m_fMoveSpeed * Time.deltaTime);


            m_fMoveSpeed -= 1 * Time.deltaTime;
        }
    }
}
