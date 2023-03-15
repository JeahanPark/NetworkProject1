using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using Sygnal;
public enum SygnalEventType
{
    NONE,
    TEST,
    Ingame_UpdatetMySkill,
}


// �Ű����� �߰��ؾ߉�
// ������ �ʿ������ ���߿� �߰�����
public class SygnalManager : Singleton<SygnalManager>
{
    private Dictionary<SygnalEventType, SygnalObserver> m_dicSygnal = new Dictionary<SygnalEventType, SygnalObserver>();

    public override void Destroy()
    {
    }

    public override void Init()
    {
    }

    public void Register(SygnalEventType _sygnalEvent, Action _sygnalFun)
    {
        SygnalObserver sygnal = null;

        // �ִ��� üũ
        if (!m_dicSygnal.TryGetValue(_sygnalEvent, out sygnal))
        {
            sygnal = new SygnalObserver();
            m_dicSygnal.Add(_sygnalEvent, sygnal);
        }

        sygnal.Add(_sygnalFun);
    }
    public void Unregister(SygnalEventType _sygnalEvent, Action _sygnalFun)
    {
        SygnalObserver sygnal = null;

        // �ִ��� üũ
        if (!m_dicSygnal.TryGetValue(_sygnalEvent, out sygnal))
        {
            // ����.
            return;
        }

        sygnal.Pop(_sygnalFun);
    }

    public void Fire(SygnalEventType _sygnalEvent)
    {
        SygnalObserver sygnal = null;

        // �ִ��� üũ
        if (m_dicSygnal.TryGetValue(_sygnalEvent, out sygnal))
        {
            sygnal.ExcuteAllFuntion();
        }
    }
}

namespace Sygnal
{
    public class SygnalObserver
    {
        private List<Action> actions = new List<System.Action>();

        public void Add(Action action)
        {
            // �Ȱ��� �ν��Ͻ��� �Լ��� �ִ��� üũ
            for (int i = 0; i < actions.Count; ++i)
            {
                if (actions[i] == action)
                    return;
            }

            actions.Add(action);
        }

        public void Pop(Action action)
        {
            for (int i = 0; i < actions.Count; ++i)
            {
                if (actions[i] == action)
                {
                    actions.Remove(action);
                }
            }
        }

        public void ExcuteAllFuntion()
        {
            for (int i = 0; i < actions.Count; ++i)
            {
                actions[i]();
            }
        }
    }
}
