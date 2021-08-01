#include "pch.h"
#include "AccountMananger.h"
#include "UserManager.h"
void AccountMananger::ProcessLogin()
{
	// userLock
	User* user = UserManager::Instance()->GetUser(100);

	// accountLock
	lock_guard<mutex> guard(_mutex);
}
