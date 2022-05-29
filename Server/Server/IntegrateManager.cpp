#include "pch.h"
#include "IntegrateManager.h"
#include "ThreadManager.h"
#include "ListenerSocket.h"

ThreadManager* g_ThreadManager = nullptr;
ListenerSocket* g_ListenerSocket = nullptr;

class IntegrateManager
{
public:
	IntegrateManager()
	{
		g_ThreadManager = new ThreadManager();
		g_ListenerSocket = new ListenerSocket();
	}
	~IntegrateManager()
	{
		delete g_ThreadManager;
		delete g_ListenerSocket;
	}
} g_IntergrateManager;