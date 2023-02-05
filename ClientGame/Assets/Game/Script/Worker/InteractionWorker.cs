using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InteractionWorker : MonoBehaviour
{
    private UserObject m_originUser = null;

    private Queue<UserObject> m_poolUserObject = null;

    private LinkedList<InteractionObject> m_ActiveObject = null;


    private void Awake()
    {
        InGameController.Instance.SetInteractionWorker(this);

        Initialize();
    }

    private void Initialize()
    {
        m_originUser = transform.Find<UserObject>("OriginUser");
        m_originUser.gameObject.SetActive(false);

        m_poolUserObject = new Queue<UserObject>();

        m_ActiveObject = new LinkedList<InteractionObject>();
    }
    public void SetInitialInGameData(InitialInGameDataPacket _packet, InitialInGameData[] _interationInitDatas)
    {
        for (int i = 0; i < _interationInitDatas.Length; ++i)
        {
            InteractionData data = _interationInitDatas[i].m_UserData;
            var begine = m_ActiveObject.First;

            InteractionObject interaction = null;
            for (var iter = begine; iter != null;)
            {
                // 같은 
                if (iter.Value.IsSameInteraction(data))
                {
                    interaction = iter.Value;

                    // Update
                    interaction.UpdateInteraction(data);

                    break;
                }

                iter = iter.Next;
            }

            if (interaction == null)
            {
                // 없으면 만들어줘야됌
                interaction = CreateInteraction(data);
            }

            UserObject user = interaction as UserObject;

            user.SetInitialUserData(_interationInitDatas[i].m_strNickName);

            // 내꺼다
            if(user.GetInteractionIndex == _packet.m_iMyInteractionIndex)
            {
                // 워커에 세팅해준다.
                InGameController.Instance.SetMyInteraction(user);
            }
        }
    }


    public void UpdateInteraction(InGameUpdatePacket _inGameUpdatePacket, InteractionData[] _interactionPacketDatas)
    {
        for ( int i = 0; i < _interactionPacketDatas.Length; ++i)
        {
            InteractionData data = _interactionPacketDatas[i];
            var begine = m_ActiveObject.First;

            InteractionObject interaction = null;
            for ( var iter = begine; iter != null;)
            {
                // 같은 
                if (iter.Value.IsSameInteraction(data))
                {
                    interaction = iter.Value;

                    // Update
                    interaction.UpdateInteraction(data);
                    
                    // 삭제
                    if (interaction.IsDelete())
                    {
                        DeleteInteraction(iter);
                    }
                    break;
                }

                iter = iter.Next;
            }

            if(interaction == null)
            {
                // 없으면 만들어줘야됌
                interaction = CreateInteraction(data);
            }

        }
    }

    private InteractionObject CreateInteraction(InteractionData _data)
    {
        InteractionObject interactionObject = null;

        switch (_data.m_eType)
        {
            case eInteractionType.User:
                {
                    if(m_poolUserObject.Count == 0)
                        interactionObject = CreateUser();
                    else
                        interactionObject = m_poolUserObject.Dequeue();
                }
                break;
        }
        m_ActiveObject.AddLast(interactionObject);

        // 초기화
        interactionObject.Initialize(_data.m_eType, _data.m_iInteractionIndex);

        // Update
        interactionObject.UpdateInteraction(_data);

        interactionObject.gameObject.SetActive(true);

        return interactionObject;
    }

    private UserObject CreateUser()
    {
        if (m_originUser == null)
            return null;

        UserObject user = Instantiate<UserObject>(m_originUser);
        user.gameObject.SetActive(true);
        return user;
    }

    private void DeleteInteraction(LinkedListNode<InteractionObject> _interaction)
    {
        InteractionObject interactionObject = _interaction.Value;

        interactionObject.gameObject.SetActive(false);

        m_ActiveObject.Remove(_interaction);

        switch(interactionObject.GetInteractionType)
        {
            case eInteractionType.User:
                UserObject user = interactionObject as UserObject;
                m_poolUserObject.Enqueue(user);
                break;
        }

        interactionObject.Clear();
    }
}
