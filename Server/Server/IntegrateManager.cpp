#include "pch.h"
#include "IntegrateManager.h"

#include "ListenerSession.h"


ThreadManager* g_ThreadManager = nullptr;
SessionManager* g_SessionManager = nullptr;

class IntegrateManager
{
public:
	IntegrateManager()
	{
		g_ThreadManager = new ThreadManager();
		g_SessionManager = new SessionManager();
	}
	~IntegrateManager()
	{
		delete g_ThreadManager;
		delete g_SessionManager;
	}
} g_IntergrateManager;