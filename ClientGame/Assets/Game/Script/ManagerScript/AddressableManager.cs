using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AddressableAssets;
using UnityEngine.ResourceManagement.AsyncOperations;
using UnityEngine.ResourceManagement.ResourceLocations;
namespace AddreesableManager
{
    // 어드레서블 라벨 이름을 enum으로 지정
    public enum AddressableLabelName
    {
        Popup,
        LABEL_MAX,
    }

    public class AddressableManager : Singleton<AddressableManager>
    {
        public override void Init()
        {
            m_eManagerState = ManagerState.InitBefore;

            var async = Addressables.InitializeAsync();
            async.Completed += (op) =>
            {
                Addressables.Release(async);
                m_eManagerState = ManagerState.InitAfter;
            };
        }

        public void AddressableResourceLoad(AddressableLabelName _eAddressableLabelName, System.Action<IList<IResourceLocation>> _fnResult)
        {
            if (m_eManagerState != ManagerState.InitAfter)
            {
                _fnResult(null);
                return;
            }
                
            Addressables.LoadResourceLocationsAsync(_eAddressableLabelName.ToString()).Completed += (handle) =>
            {
                _fnResult(handle.Result);
            };
        }

        public void InstanceIResourceLocation(IResourceLocation _resourceLocation, Transform _trParent, System.Action<GameObject> _fnResult)
        {
            Addressables.InstantiateAsync(_resourceLocation, _trParent, true).Completed += (handle) =>
            {
                _fnResult(handle.Result);
            };
        }
        public override void Destroy()
        {

        }
    }
}
