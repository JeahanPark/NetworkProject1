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
    /// ���ʰ� �ڽ��� ������Ʈ�� �� ������ ���¿��� ȣ��
    /// ������Ʈ�� ������ ������ �����ϰ� ����ϱ����� ȣ��
    /// </summary>
    public virtual void InitComponent()
    {

    }
}
