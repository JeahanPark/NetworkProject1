using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.ResourceManagement.ResourceLocations;
using AddreesableManager;

public enum EffectID
{
    NONE,
    UserBody_Normal,
    UserBody_Damage,

    PrefabEffect,
    DieEffect,
    Effect_MAX
}


public class EffectManager : MonoSingleton<EffectManager>
{
    private Dictionary<EffectID, IResourceLocation> m_dicResourceLocaation = null;

    private Dictionary<EffectID, BaseEffect> m_originEffect = null;

    private Dictionary<EffectID, Queue<BaseEffect>> m_PoolBaseEffect = null;

    private GameObject m_poolParent = null;

    public override void Destroy()
    {
    }

    public override void Init()
    {
        StartCoroutine(InitProcessor());
    }

    private IEnumerator InitProcessor()
    {
        m_eManagerState = ManagerState.InitBefore;

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
            m_dicResourceLocaation = new Dictionary<EffectID, IResourceLocation>();
            for (int i = 0; i < lisResult.Count; ++i)
            {
                foreach (EffectID id in System.Enum.GetValues(typeof(EffectID)))
                {
                    if (id.ToString() == lisResult[i].PrimaryKey && !m_dicResourceLocaation.ContainsKey(id))
                    {
                        m_dicResourceLocaation.Add(id, lisResult[i]);
                        break;
                    }
                }
            }
        }

        m_originEffect = new Dictionary<EffectID, BaseEffect>();
        m_PoolBaseEffect = new Dictionary<EffectID, Queue<BaseEffect>>();

        int startIndex = (int)EffectID.PrefabEffect;
        int endIndex = (int)EffectID.Effect_MAX;
        for ( int i = startIndex; i < endIndex; ++i)
        {
            EffectID effectID = (EffectID)i;
            AddressableManager.Instance.InstanceIResourceLocation(m_dicResourceLocaation[effectID], transform, (_Result) => 
            {
                m_PoolBaseEffect.Add(effectID, new Queue<BaseEffect>());

                BaseEffect effect = _Result.GetComponent<BaseEffect>();
                m_originEffect.Add(effectID, effect);
            });
        }

        m_poolParent = new GameObject("EffectPool");
        m_poolParent.transform.SetParent(transform);
        m_poolParent.SetActive(false);

        m_eManagerState = ManagerState.InitSuccess;
    }

    public void GetEffect<T>(EffectID _id, System.Action<T> _fnAsyncResult) where T : class
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

    public BaseEffect GetEffect(EffectID _id, Transform _parent = null)
    {
        BaseEffect effect = CreateEffect(_id);

        if (effect != null && _parent != null)
            effect.transform.SetParent(_parent);

        return effect;
    }

    public void UnUseEffect(BaseEffect _effect)
    {
    }
    private BaseEffect CreateEffect(EffectID _id)
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
