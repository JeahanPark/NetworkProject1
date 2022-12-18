using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;

public enum SceneType
{
    NONE,
    Intro,
    Lobby,
    InGame,
}
public class SceneManager : Singleton<SceneManager>
{
    private SceneType m_eCurrentScene = SceneType.NONE;

    public override void Init()
    {
        m_eManagerState = ManagerState.InitAfter;
    }
    public override void Destroy()
    {

    }

    public void SceneChange(SceneType _eSceneType)
    {
        m_eCurrentScene = _eSceneType;
        string SceneName = _eSceneType.ToString();
        UnityEngine.SceneManagement.SceneManager.LoadScene(SceneName);
    }

    public SceneType GetCurentScene()
    {
        return m_eCurrentScene;
    }
}
