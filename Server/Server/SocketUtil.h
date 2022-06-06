#pragma once
#include "SocketEvent.h"

class SocketUtil
{
public:
	static SOCKET SocketCreate()
	{
		return ::socket(AF_INET, SOCK_STREAM, 0);
	}
};