using System.Collections.Generic;
using System.Text;
using UnityEditor;
using UnityEngine;
using UnityEngine.UI;

public static class UIFrameworkTool
{
    class ChildData
    {
        public System.Type dataType;
        public GameObject targetObject;
        public int transInstanceID;

        public string GetFieldName()
        {
            return GenerateFieldName(targetObject.name);
        }

        static private string GenerateFieldName(string _name)
        {
            string fieldName = _name;

            // 변수명에 공백이 있는 경우, 언더바로 대체.
            fieldName = fieldName.Replace(" ", "_");

            // 변수명에 괄호가 있는 경우, 제거.
            fieldName = fieldName.Replace("(", string.Empty);
            fieldName = fieldName.Replace(")", string.Empty);

            // 변수명이 숫자로 시작하는경우 언더바 추가.
            if (fieldName.Length > 0 && char.IsNumber(fieldName, 0))
                fieldName = "_" + fieldName;

            return fieldName;
        }
    }

    // 하이어라키에서 A에서 B오브젝트를 Find해주는 코드 생성
    [MenuItem("GameObject/ToolExtension/Generate Find Code Ctrl+Q %q", false, -9)]
    static void GenerateFindChildCode()
    {
        if (Selection.gameObjects.Length <= 1)
        {
            return;
        }

        // 컴포넌트 우선순위.
        System.Type[] comp_priority_list = new System.Type[]{
            typeof(Slider),
            typeof(InputField),
            typeof(Button),
            typeof(Text),
            typeof(Image),
            typeof(Grid),
            typeof(SpriteRenderer),
            typeof(CanvasGroup),
            typeof(UnityEngine.RectTransform),
            typeof(UnityEngine.Transform),
            };

        // 컴포넌트 우선순위 찾는 함수.
        System.Func<GameObject, System.Type> FindCompTypeFunc = delegate (GameObject target)
        {
            foreach (var one in comp_priority_list)
            {
                Component comp = target.GetComponent(one);
                if (comp != null)
                {
                    Behaviour behaviour = comp as Behaviour;
                    if (behaviour != null && behaviour.enabled) //활성화된 컴포넌트만 체크.
                        return one;
                }
            }
            return typeof(Transform);
        };//end, FindCompTypeFunc

        // 선택된 오브젝트들을 ChildData 로 래핑.
        GameObject rootObj = null;
        GameObject[] gameObjects = Selection.GetFiltered<GameObject>(SelectionMode.TopLevel);
        if(gameObjects.Length > 1)
        {
            Debug.Log("탑 오브젝트가 2개있다.");
            return;
        }
        rootObj = gameObjects[0];

        List<ChildData> notSortedDataList = new List<ChildData>();
        foreach (GameObject go in Selection.gameObjects)
        {
            if (rootObj == go)
                continue;
            

            ChildData di = new ChildData();
            di.dataType = FindCompTypeFunc(go);
            di.targetObject = go;
            di.transInstanceID = go.transform.GetInstanceID();

            notSortedDataList.Add(di);

        }

        // 루트오브젝트 체크: Selection.activeGameObject;
        if (rootObj == null)
        {
            Debug.Log("[UIFrameworkTool.GenerateFindChildCode] rootObj is null");
            return;
        }
        Debug.Log("[UIFrameworkTool.GenerateFindChildCode] rootObj name " + rootObj.name);

        // 하이러키 순서대로 소팅.
        List<ChildData> sortedDataList = new List<ChildData>();
        RecursivelySortChildData(rootObj.transform, notSortedDataList, sortedDataList);

        // 트랜스폼 찾는 코드 생성.
        StringBuilder sb = new StringBuilder();
        StringBuilder temp = new StringBuilder();

        foreach (ChildData data in sortedDataList)
        {
            string type = data.dataType.ToString();
            type = type.Replace("UnityEngine.", "");
            type = type.Replace("UI.", "");
            type = type.Replace("Mocatree.", "");
            string hierarchyPath = GetHierarchyPath(data.targetObject, "/", rootObj.transform);

            string fieldName = data.GetFieldName();
            sb.AppendFormat("{1} _{0} = transform.Find<{1}>(\"{2}\");\n", fieldName, type, hierarchyPath);
        }//end, for

        // 클립보드에 카피.
        if (sb.Length > 0)
        {
            ClipBoard = sb.ToString();
        }

    }

    static void RecursivelySortChildData(Transform _root, List<ChildData> _notSortedData, List<ChildData> _sortedData)
    {
        if (_root == null)
        {
            Debug.LogError("RecursivelySortChildData() root is null");
            return;
        }

        int rootInstID = _root.GetInstanceID();
        ChildData findData = _notSortedData.Find(it => it.transInstanceID == rootInstID);
        if (findData != null)
        {
            _sortedData.Add(findData);
        }

        for (int i = 0; i < _root.childCount; ++i)
        {
            RecursivelySortChildData(_root.GetChild(i), _notSortedData, _sortedData);
        }
    }
    public static string GetHierarchyPath(GameObject obj, string token = "/", Transform top_most = null)
    {
        if (obj == null) return "";
        string path = obj.name;

        if (obj.transform == top_most)
            return path;

        while (obj.transform.parent != null && obj.transform.parent != top_most)
        {
            obj = obj.transform.parent.gameObject;
            path = obj.name + token + path;
        }
        return path;
    }

    public static string ClipBoard
    {
        get
        {
            return GUIUtility.systemCopyBuffer;
        }
        set
        {
            GUIUtility.systemCopyBuffer = value;
        }
    }
}