#pragma once

#include <iostream>
#include <stdio.h>
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
#include <list>
#include <string>
#include <set>
#include <queue>
#include <map>
#include <DirectXMath.h>
using namespace DirectX;
//잡다한거

// 내가만든거
#include "SereverMacro.h"
#include "Types.h"
#include "TLSData.h"
#include "IntegrateManager.h"
#include "Session.h"
#include "SocketUtil.h"
#include "ThreadManager.h"
#include "PacketData.h"
#include "SendBuffer.h"
#include "BaseManager.h"
// 내가만든거