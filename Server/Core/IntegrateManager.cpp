#include "pch.h"
#include "IntegrateManager.h"



ThreadManager* g_ThreadManager = nullptr;

class IntegrateManager
{
public:
	IntegrateManager()
	{
		g_ThreadManager = new ThreadManager();
	}
	~IntegrateManager()
	{
		delete g_ThreadManager;
	}
} g_IntergrateManager;