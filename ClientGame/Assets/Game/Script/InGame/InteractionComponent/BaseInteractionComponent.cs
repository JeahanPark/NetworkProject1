using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public enum eInteractionCom
{
    NONE,
    Info,
    UserInfo,
    UI,
}
public class BaseInteractionComponent
{
    public BaseInteractionComponent(InteractionObject _ownerInteraction)
    {
        m_ownerInteraction = _ownerInteraction;
    }
    protected InteractionObject m_ownerInteraction = null;

    protected eInteractionCom m_eInteractionCom;
    public eInteractionCom InteractionComType
    {
        get
        {
            return m_eInteractionCom;
        }
    }

    /// <summary>
    /// 오너가 자신의 컴포넌트를 다 세팅한 상태에서 호출
    /// 컴포넌트가 오너의 데이터 안전하게 사용하기위해 호출
    /// </summary>
    public virtual void InitComponent()
    {

    }
}
