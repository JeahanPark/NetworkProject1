using System.Collections;
using System.Collections.Generic;
using UnityEngine;




public class InfoCom : BaseInteractionComponent
{
    public InfoCom(InteractionObject _ownerInteraction) : base(_ownerInteraction)
    {
        m_eInteractionCom = eInteractionCom.Info;
    }

    // InfoCom은 Interaction의 데이터를 보관을 하기위한 객체다.
    // 닉네임, 체력 등
}
