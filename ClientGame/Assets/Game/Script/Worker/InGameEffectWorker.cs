using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.ResourceManagement.ResourceLocations;
using AddreesableManager;


public class InGameEffectWorker : MonoBehaviour
{
    private Dictionary<EffectType, IResourceLocation> m_dicResourceLocaation = null;

    private Dictionary<EffectType, BaseEffect> m_originEffect = null;

    private Dictionary<EffectType, Queue<BaseEffect>> m_PoolBaseEffect = null;

    private GameObject m_poolParent = null;

    private void Awake()
    {
        InGameController.Instance.SetInGameEffectWorker(this);

        m_poolParent = transform.Find("UnUsePool").gameObject;

    }

    private void Start()
    {
        InitEffect();
    }

    public void InitEffect()
    {
        StartCoroutine(InitProcessor());
    }

    private IEnumerator InitProcessor()
    {
        IList<IResourceLocation> lisResult = null;

        bool bResult = false;

        AddressableManager.Instance.AddressableResourceLoad(AddressableLabelName.Effect, (_lisResult) =>
        {
            lisResult = _lisResult;
            bResult = true;
        });

        yield return new WaitUntil(() => { return bResult; });

        if (lisResult == null || lisResult.Count == 0)
        {
            Debug.LogWarning("Popup IResourceLocation Null");
        }
        else
        {
            m_dicResourceLocaation = new Dictionary<EffectType, IResourceLocation>();
            for (int i = 0; i < lisResult.Count; ++i)
            {
                foreach (EffectType id in System.Enum.GetValues(typeof(EffectType)))
                {
                    if (id.ToString() == lisResult[i].PrimaryKey && !m_dicResourceLocaation.ContainsKey(id))
                    {
                        m_dicResourceLocaation.Add(id, lisResult[i]);
                        break;
                    }
                }
            }
        }

        m_originEffect = new Dictionary<EffectType, BaseEffect>();
        m_PoolBaseEffect = new Dictionary<EffectType, Queue<BaseEffect>>();

        int startIndex = (int)EffectType.PrefabEffect + 1;
        int endIndex = (int)EffectType.Effect_MAX;
        for ( int i = startIndex; i < endIndex; ++i)
        {
            EffectType effectID = (EffectType)i;
            AddressableManager.Instance.InstanceIResourceLocation(m_dicResourceLocaation[effectID], transform, (_Result) => 
            {
                m_PoolBaseEffect.Add(effectID, new Queue<BaseEffect>());

                BaseEffect effect = _Result.GetComponent<BaseEffect>();
                m_originEffect.Add(effectID, effect);
            });
        }
    }

    public void GetEffect<T>(EffectType _id, System.Action<T> _fnAsyncResult) where T : class
    {
        IResourceLocation resourceLocation = m_dicResourceLocaation[_id];

        if (resourceLocation == null)
        {
            Debug.LogError("พ๘ดย effect");
            _fnAsyncResult(null);
            return;
        }

        AddressableManager.Instance.AssetAsyncIResourceLocation<T>(resourceLocation, _fnAsyncResult);
    }

    public BaseEffect GetPrefabEffect(EffectType _id, Vector3 _vPos)
    {
        BaseEffect effect = CreateEffect(_id);

        if (effect != null)
        {
            effect.transform.position = _vPos;
        }
            

        return effect;
    }

    public void UnUseEffect(BaseEffect _effect)
    {
        if (_effect == null)
            return;

        m_PoolBaseEffect[_effect.GetEffectType].Enqueue(_effect);
    }
    private BaseEffect CreateEffect(EffectType _id)
    {
        if (!m_originEffect.ContainsKey(_id))
            return null;

        BaseEffect effect = null;
        Queue<BaseEffect> pool = m_PoolBaseEffect[_id];

        if(pool.Count == 0)
        {
            effect = Instantiate(m_originEffect[_id], m_poolParent.transform, false);
        }
        else
        {
            effect = pool.Dequeue();
        }

        return effect;
    }
}
