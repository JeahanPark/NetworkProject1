using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InteractionWorker : MonoBehaviour
{
    private UserObject m_originUser = null;
    private AttackDummy m_originAttackDummy = null;
    private FireBall m_originFireBall = null;
    private RefletionItem m_originReflectionItem = null;

    private Dictionary<eInteractionType, Queue<InteractionObject>> m_poolObject = null;
    private Dictionary<eInteractionType, LinkedList<InteractionObject>> m_mapActiveObject = null;

    private Transform m_UnUseUserPool = null;


    private void Awake()
    {
        InGameController.Instance.SetInteractionWorker(this);

        Initialize();
    }

    private void Initialize()
    {
        m_originUser = transform.Find<UserObject>("OriginUser");
        m_originUser.gameObject.SetActive(false);

        m_originAttackDummy = transform.Find<AttackDummy>("OriginAttackDummy");
        m_originAttackDummy.gameObject.SetActive(false);

        m_originFireBall = transform.Find<FireBall>("OriginFireBall");
        m_originFireBall.gameObject.SetActive(false);

        m_originReflectionItem = transform.Find<RefletionItem>("OriginReflectionItem");
        m_originReflectionItem.gameObject.SetActive(false);

        m_UnUseUserPool = transform.Find<Transform>("UnUseUserPool");

        m_poolObject = new Dictionary<eInteractionType, Queue<InteractionObject>>();
        m_mapActiveObject = new Dictionary<eInteractionType, LinkedList<InteractionObject>>();
    }
    public void AddNewUser(NewUserPacket _packet)
    {
        InteractionData data = _packet.m_InitData.m_UserData;
        var begine = GetActiveObjectList(eInteractionType.User).First;

        InteractionObject interaction = null;
        for (var iter = begine; iter != null;)
        {
            // ���� 
            if (iter.Value.IsSameInteraction(data.m_iInteractionIndex))
            {
                interaction = iter.Value;

                break;
            }

            iter = iter.Next;
        }

        if (interaction == null)
        {
            // ������ �������߉�
            interaction = CreateInteraction(data);
            if (interaction == null)
                return;
        }

        interaction.UpdateInteraction(data, 0);

        UserObject user = interaction as UserObject;

        user.SetInitialUserData(_packet.m_InitData.m_strNickName);
    }

    public void SetInitialInGameData(InitialInGameDataPacket _packet, InitialInGameData[] _interationInitDatas)
    {
        for (int i = 0; i < _interationInitDatas.Length; ++i)
        {
            InteractionData data = _interationInitDatas[i].m_UserData;
            var begine = GetActiveObjectList(eInteractionType.User).First;

            InteractionObject interaction = null;
            for (var iter = begine; iter != null;)
            {
                // ���� 
                if (iter.Value.IsSameInteraction(data.m_iInteractionIndex))
                {
                    interaction = iter.Value;
                    break;
                }

                iter = iter.Next;
            }

            if (interaction == null)
            {
                // ������ �������߉�
                interaction = CreateInteraction(data);
                if (interaction == null)
                    continue;
            }

            UserObject user = interaction as UserObject;

            user.SetInitialUserData(_interationInitDatas[i].m_strNickName);

            // ������
            if(user.GetInteractionIndex == _packet.m_iMyInteractionIndex)
            {
                // ��Ŀ�� �������ش�.
                InGameController.Instance.SetMyInteraction(user);
            }
        }
    }

    public void UserRiseAgain(InitialInGameData _initialInGameData)
    {
        UserObject user = CreateInteraction(_initialInGameData.m_UserData) as UserObject;

        user.SetInitialUserData(_initialInGameData.m_strNickName);

        InGameController.Instance.SetMyInteraction(user);
    }
    public void UpdateInteraction(InGameUpdatePacket _inGameUpdatePacket, InteractionData[] _interactionPacketDatas, float _fUpdateLatency)
    {
        for ( int i = 0; i < _interactionPacketDatas.Length; ++i)
        {
            InteractionData data = _interactionPacketDatas[i];

            InteractionObject interaction = null;
            var iter = GetActiveObjectList(data.m_eType).First;
            for ( ;iter != null;)
            {
                // ���� 
                if (iter.Value.IsSameInteraction(data.m_iInteractionIndex))
                {
                    interaction = iter.Value;
                    break;
                }

                iter = iter.Next;
            }

            if (interaction == null)
            {
                // ������ �������߉�
                interaction = CreateInteraction(data);
                if (interaction == null)
                    continue;
            }

            // Update
            interaction.UpdateInteraction(data, _fUpdateLatency);

            // ���̻� ��ȿ���� ���� ��ü��.
            if (!interaction.GetValidLife)
            {
                if(iter == null)
                {
                    // iter�� null�ΰ��� Ŭ�󿡴� ���� ������������ interaction�� �ִµ�
                    // �������ؾߵǴ� �����ֵ� �����ؾߵǴ°��
                    DeleteInteraction(interaction);
                }
                else
                {
                    DeleteInteraction(iter);
                }
                
            }
        }
    }

    public void RecivedDamage(RecivedDamagePacket _packet)
    {
        var begine = GetActiveObjectList(eInteractionType.User).First;

        InteractionObject interaction = null;
        for (var iter = begine; iter != null;)
        {
            // ���� 
            if (iter.Value.IsSameInteraction(_packet.m_iInteractionIndex))
            {
                interaction = iter.Value;
                interaction.RecivedDamage(_packet.m_fReciveDamage);
                break;
            }

            iter = iter.Next;
        }
    }

    public void ShowSkill(ShowSkillPacket _packet)
    {
        var lisUser = GetActiveObjectList(eInteractionType.User);

        foreach(var iter in lisUser)
        {
            if(iter.GetInteractionIndex == _packet.m_iInteractionIndex)
            {
                UserObject user = iter as UserObject;

                user.ShowSkill(_packet.m_eSkillType);
                break;
            }
        }
    }

    private LinkedList<InteractionObject> GetActiveObjectList(eInteractionType _eInteractionType)
    {
        LinkedList<InteractionObject> list = null;
        if(!m_mapActiveObject.TryGetValue(_eInteractionType, out list))
        {
            list = new LinkedList<InteractionObject>();
            m_mapActiveObject.Add(_eInteractionType, list);
        }

        return list;
    }
    private Queue<InteractionObject> GetPoolQueue(eInteractionType _eInteractionType)
    {
        Queue<InteractionObject> queue = null;
        if (!m_poolObject.TryGetValue(_eInteractionType, out queue))
        {
            queue = new Queue<InteractionObject>();
            m_poolObject.Add(_eInteractionType, queue);
        }

        return queue;
    }
    private InteractionObject CreateInteraction(InteractionData _data)
    {
        InteractionObject interactionObject = null;

        if(_data.m_eType == eInteractionType.AttackDummy)
        {
            interactionObject = Instantiate<AttackDummy>(m_originAttackDummy);
            interactionObject.gameObject.SetActive(true);
        }
        else
        {
            Queue<InteractionObject> pool = GetPoolQueue(_data.m_eType);
            if (pool.Count == 0)
                interactionObject = GetOriginInteraction(_data.m_eType);
            else
            {
                interactionObject = pool.Dequeue();
                interactionObject.transform.SetParent(null);
            }
        }

        if (interactionObject == null)
            return null;

        GetActiveObjectList(_data.m_eType).AddLast(interactionObject);

        // �ʱ�ȭ
        interactionObject.Initialize(_data);

        return interactionObject;
    }

    private InteractionObject GetOriginInteraction(eInteractionType _type)
    {
        InteractionObject origin = null;

        switch(_type)
        {
            case eInteractionType.AttackFireBall:
                origin = m_originFireBall;
                break;
            case eInteractionType.ReflectionItem:
                origin = m_originReflectionItem;
                break;
            case eInteractionType.User:
                origin = m_originUser;
                break;
        }

        if (origin == null)
            return null;

        InteractionObject user = Instantiate(origin);
        user.gameObject.SetActive(true);
        return user;
    }

    private void DeleteInteraction(LinkedListNode<InteractionObject> _interaction)
    {
        InteractionObject interactionObject = _interaction.Value;

        GetActiveObjectList(interactionObject.GetInteractionType).Remove(_interaction);

        DeletePool(interactionObject);
    }

    private void DeleteInteraction(InteractionObject _interaction)
    {
        var list = GetActiveObjectList(_interaction.GetInteractionType);
        
        for(var iter = list.First; iter != null;)
        {
            if (iter.Value.GetInteractionIndex == _interaction.GetInteractionIndex)
            {
                var deleteIter = iter;
                iter = iter.Next;

                list.Remove(deleteIter);
            }
            else
                iter = iter.Next;
        }

        //foreach (var interaction in list)
        //{
        //    if(interaction.GetInteractionIndex == _interaction.GetInteractionIndex)
        //    {
        //        list.Remove(interaction);
        //    }
        //}

        DeletePool(_interaction);
    }

    private void DeletePool(InteractionObject _interaction)
    {
        var pool = GetPoolQueue(_interaction.GetInteractionType);

        if(_interaction.GetInteractionType == eInteractionType.AttackDummy)
        {
            Destroy(_interaction.gameObject);
            _interaction = null;
        }
        else
        {
            pool.Enqueue(_interaction);
            _interaction.transform.SetParent(m_UnUseUserPool.transform);
        }

        if (_interaction != null)
        {
            // �׾����� ���õ� �˾�, ����Ʈ ����
            _interaction.Die();

            // ������ Ŭ����
            _interaction.Clear();

            // ��ġ �Ⱥ��̴µ��� �ű��
            _interaction.transform.position = new Vector3(0, 999, 0);
        }
    }
}
