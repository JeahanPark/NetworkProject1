#pragma once

#include <thread>
#include <functional>


class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void Run(function<void(void)> _fnCallback);
	void Join();

private:
	void InitTLS();
	void DestoryTLS();

private:
	mutex m_lock;
	vector<thread> m_vecThread;
};

