using System.Collections;
using System.Collections.Generic;
using UnityEngine;




public class InfoCom : BaseInteractionComponent
{
    public InfoCom(InteractionObject _ownerInteraction) : base(_ownerInteraction)
    {
        m_eInteractionCom = eInteractionCom.Info;
    }

    // InfoCom�� Interaction�� �����͸� ������ �ϱ����� ��ü��.
    // �г���, ü�� ��
}
