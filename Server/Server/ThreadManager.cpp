#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Run(function<void(void)> _fnCallback)
{
	LockGuard lock(m_lock);

	m_vecThread.push_back(thread([=]()
	{
		InitTLS();
		_fnCallback();
		DestoryTLS();
	}));
}

void ThreadManager::Join()
{
	for (thread& t : m_vecThread)
	{
		if(t.joinable())
			t.join();
	}
}

void ThreadManager::InitTLS()
{
	static int nThreadID = 0;
	g_ThreadID = nThreadID++;
}

void ThreadManager::DestoryTLS()
{
}
