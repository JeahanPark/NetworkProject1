using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InteractionObject : MonoBehaviour
{
    protected eInteractionType m_eInteractionType = eInteractionType.None;

    protected int m_iInteractionIndex = EnumType.InteractionIndexNone;

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

    public bool IsSameInteraction(InteractionPacketData _interactionPacketData)
    {
        Debug.Log(_interactionPacketData.m_eType.ToString() + ","
            + GetInteractionType.ToString() + "," 
            + _interactionPacketData.m_iInteractionIndex.ToString() + ","
            + GetInteractionIndex.ToString());
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

    public void UpdatePos(Vector3 _vPos)
    {
        transform.position = _vPos;
    }

    public bool IsDelete()
    {
        return false;
    }
}
