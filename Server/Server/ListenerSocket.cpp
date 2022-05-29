#include "pch.h"
#include "ListenerSocket.h"

ListenerSocket::ListenerSocket()
{
	Init();
}

ListenerSocket::~ListenerSocket()
{
}

void ListenerSocket::Init()
{
	// 윈도우 소켓 생성
	{
		WORD wVersionRequested;
		WSAData wsaData;

		wVersionRequested = MAKEWORD(2, 2);

		if (::WSAStartup(wVersionRequested, &wsaData) != 0)
		{
			CRASH("WSAStartup Fail");
			return;
		}
	}


	//소켓 클래스는 네트워크 통신을 하기 위해 필요한 함수나 변수를 제공해주는 클래스입니다.
	m_listenSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN serverAddr;
	{
	    // SOCKADDR_IN
	    // 소켓의 주소를 표현하는 구조체다.
	    // 범용 주소 구조체인 SOCKADDR를 사용하지만 주소체계에 따라 전용 구조체를 만든다
	    // SOCKADDR_IN은 IPv4 주소체계 전용으로 사용하기 위한 구조체다.
	    
	    memset(&serverAddr, 0, sizeof(serverAddr));
	
	    serverAddr.sin_family = AF_INET;
	    serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	    serverAddr.sin_port = ::htons(7777);
	}

	// bind 함수 는 소켓 주소 표현 구조체와 소켓을 연결하기위해 사용하는것
	if (::bind(m_listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		CRASH("bind Fail");
	    return;
	}

	// 다른 프로세스?(ex.클라이언트)에서 연결요청을 했을경우 처리가 가능한 상태
	if (::listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		CRASH("listen Fail");
	    return;
	}

	//    // CreateIoCompletionPort함수는 2가지 용도로 사용한다.
	// 1. IOCP커널 오브젝트를 생성하기위해 사용된다.
	//    처음에 한번 호출을 해줘야한다.
	//    호출후에는 IOCP 핸들을 반환한다.
	// 2. IOCP 완료 포트 핸들과 소켓 핸들을 연결하면 프로세스에서 
	//    해당 소켓 핸들과 관련된 비동기 I/O 작업의 완료 알림을 받을수 있다.
	
	m_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);


	// 
	DWORD bytes = 0;

	GUID guid_CONNECTEX = WSAID_CONNECTEX;
	IF_CRASH(SOCKET_ERROR == ::WSAIoctl(m_listenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid_CONNECTEX, sizeof(guid_CONNECTEX), &m_fnConnectEX, sizeof(m_fnConnectEX), OUT & bytes, NULL, NULL));
	
	GUID guid_DISCONNECTEX = WSAID_DISCONNECTEX;
	IF_CRASH(SOCKET_ERROR == ::WSAIoctl(m_listenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid_DISCONNECTEX, sizeof(guid_DISCONNECTEX), &m_fnConnectEX, sizeof(m_fnConnectEX), OUT & bytes, NULL, NULL));
	
	GUID guid_ACCEPTEX = WSAID_ACCEPTEX;
	IF_CRASH(SOCKET_ERROR == ::WSAIoctl(m_listenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid_ACCEPTEX, sizeof(guid_ACCEPTEX), &m_fnConnectEX, sizeof(m_fnConnectEX), OUT & bytes, NULL, NULL));

	cout << "ListenerSocket Init Success" << endl;

	AcceptPaket* packet = new AcceptPaket();

	SessionAccept(packet);
}

void ListenerSocket::SessionAccept(AcceptPaket* _acceptPacket)
{
	Session* session = new Session();
	_acceptPacket->SetSession(session);

	DWORD bytesReceived = 0;

	if (false == m_fnAcceptEX(m_listenSocket, session->GetSocket(), session->m_recvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived, static_cast<LPOVERLAPPED>(_acceptPacket)))
	{
		const int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			CRASH(errorCode);
		}
	}
}
