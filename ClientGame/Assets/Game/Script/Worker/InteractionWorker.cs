using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InteractionWorker : MonoBehaviour
{
    private UserObject m_originUser = null;
    private AttackDummy m_originAttackDummy = null;
    private FireBall m_originFireBall = null;

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
            // 같은 
            if (iter.Value.IsSameInteraction(data.m_iInteractionIndex))
            {
                interaction = iter.Value;

                break;
            }

            iter = iter.Next;
        }

        if (interaction == null)
        {
            // 없으면 만들어줘야됌
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
                // 같은 
                if (iter.Value.IsSameInteraction(data.m_iInteractionIndex))
                {
                    interaction = iter.Value;
                    break;
                }

                iter = iter.Next;
            }

            if (interaction == null)
            {
                // 없으면 만들어줘야됌
                interaction = CreateInteraction(data);
                if (interaction == null)
                    continue;
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
                // 같은 
                if (iter.Value.IsSameInteraction(data.m_iInteractionIndex))
                {
                    interaction = iter.Value;
                    break;
                }

                iter = iter.Next;
            }

            if (interaction == null)
            {
                // 없으면 만들어줘야됌
                interaction = CreateInteraction(data);
                if (interaction == null)
                    continue;
            }

            // Update
            interaction.UpdateInteraction(data, _fUpdateLatency);

            // 더이상 유효하지 않은 객체다.
            if (!interaction.GetValidLife)
            {
                if(iter == null)
                {
                    // iter가 null인경우는 클라에는 아직 생성되지않은 interaction이 있는데
                    // 생성을해야되는 죽은애들 생성해야되는경우
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
            // 같은 
            if (iter.Value.IsSameInteraction(_packet.m_iInteractionIndex))
            {
                interaction = iter.Value;
                interaction.RecivedDamage(_packet.m_fReciveDamage);
                break;
            }

            iter = iter.Next;
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

        switch (_data.m_eType)
        {
            case eInteractionType.User:
                {
                    Queue<InteractionObject> pool = GetPoolQueue(_data.m_eType);
                    if (pool.Count == 0)
                        interactionObject = CreateUser();
                    else
                    {
                        interactionObject = pool.Dequeue();
                        interactionObject.transform.SetParent(null);
                    } 
                }
                break;
            case eInteractionType.AttackDummy:
                {
                    interactionObject = Instantiate<AttackDummy>(m_originAttackDummy);
                    interactionObject.gameObject.SetActive(true);
                }
                break;
            case eInteractionType.AttackFireBall:
                {
                    Queue<InteractionObject> pool = GetPoolQueue(_data.m_eType);
                    if (pool.Count == 0)
                    {
                        interactionObject = Instantiate<FireBall>(m_originFireBall);
                        interactionObject.gameObject.SetActive(true);
                    }
                        
                    else
                    {
                        interactionObject = pool.Dequeue();
                        interactionObject.transform.SetParent(null);
                    }
                }
                break;
        }

        if (interactionObject == null)
            return null;

        GetActiveObjectList(_data.m_eType).AddLast(interactionObject);

        // 초기화
        interactionObject.Initialize(_data);

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

        GetActiveObjectList(interactionObject.GetInteractionType).Remove(_interaction);

        DeletePool(interactionObject);
    }

    private void DeleteInteraction(InteractionObject _interaction)
    {
        var list = GetActiveObjectList(_interaction.GetInteractionType);

        foreach (var interaction in list)
        {
            if(interaction.GetInteractionIndex == _interaction.GetInteractionIndex)
            {
                list.Remove(interaction);
            }
        }

        DeletePool(_interaction);
    }

    private void DeletePool(InteractionObject _interaction)
    {
        var pool = GetPoolQueue(_interaction.GetInteractionType);

        switch (_interaction.GetInteractionType)
        {
            case eInteractionType.User:
                UserObject user = _interaction as UserObject;
                pool.Enqueue(user);
                user.transform.SetParent(m_UnUseUserPool.transform);
                break;
            case eInteractionType.AttackDummy:
                Destroy(_interaction.gameObject);
                _interaction = null;
                break;
            case eInteractionType.AttackFireBall:
                pool.Enqueue(_interaction);
                _interaction.transform.SetParent(m_UnUseUserPool.transform);
                break;
        }

        if (_interaction != null)
        {
            // 죽었을때 관련된 팝업, 이펙트 노출
            _interaction.Die();

            // 데이터 클리어
            _interaction.Clear();

            // 위치 안보이는데로 옮긴다
            _interaction.transform.position = new Vector3(0, 999, 0);
        }
    }
}
