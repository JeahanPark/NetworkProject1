using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProjectController : MonoDestroySingleton<LobbyController>
{
    public override void Destroy()
    {
        base.Destroy();
    }

    public override void Init()
    {
    }

    public void ApplicationQuit()
    {
        Application.Quit();
    }
}