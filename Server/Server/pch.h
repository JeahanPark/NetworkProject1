#pragma once


#include <iostream>
#include <stdio.h>
#include <iostream>
using namespace std;

/////////// 네트워크 관련
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#pragma comment(lib, "ws2_32.lib")
/////////// 네트워크 관련

//잡다한거
#include <vector>
#include <string>
//잡다한거

// 내가만든거
#include "SereverMacro.h"
#include "Types.h"
#include "TLSData.h"
#include "IntegrateManager.h"
#include "Session.h"
#include "Packet.h"
#include "SocketUtil.h"
#include "ThreadManager.h"
#include "SessionManager.h"
// 내가만든거