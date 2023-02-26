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
    Effect_MAX
}


public class EffectManager : MonoSingleton<EffectManager>
{
    private Dictionary<EffectID, IResourceLocation> m_dicResourceLocaation = null;

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

        AddressableManager.Instance.AssetIResourceLocation<T>(resourceLocation, _fnAsyncResult);
    }
}
