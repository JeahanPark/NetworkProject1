using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;

public enum SceneType
{
    Intro,
    Lobby,
    InGame,
    SceneChange,
}
public class SceneManager : Singleton<SceneManager>
{
    public override void Init()
    {
        m_eManagerState = ManagerState.InitAfter;
    }
    public override void Destroy()
    {

    }

    public void SceneChange(SceneType _eSceneType)
    {
        string SceneName = _eSceneType.ToString();
        UnityEngine.SceneManagement.SceneManager.LoadScene(SceneName);
    }
}
