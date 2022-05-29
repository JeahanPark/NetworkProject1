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
	// ������ ���� ����
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


	//���� Ŭ������ ��Ʈ��ũ ����� �ϱ� ���� �ʿ��� �Լ��� ������ �������ִ� Ŭ�����Դϴ�.
	m_listenSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN serverAddr;
	{
	    // SOCKADDR_IN
	    // ������ �ּҸ� ǥ���ϴ� ����ü��.
	    // ���� �ּ� ����ü�� SOCKADDR�� ��������� �ּ�ü�迡 ���� ���� ����ü�� �����
	    // SOCKADDR_IN�� IPv4 �ּ�ü�� �������� ����ϱ� ���� ����ü��.
	    
	    memset(&serverAddr, 0, sizeof(serverAddr));
	
	    serverAddr.sin_family = AF_INET;
	    serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	    serverAddr.sin_port = ::htons(7777);
	}

	// bind �Լ� �� ���� �ּ� ǥ�� ����ü�� ������ �����ϱ����� ����ϴ°�
	if (::bind(m_listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		CRASH("bind Fail");
	    return;
	}

	// �ٸ� ���μ���?(ex.Ŭ���̾�Ʈ)���� �����û�� ������� ó���� ������ ����
	if (::listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		CRASH("listen Fail");
	    return;
	}

	//    // CreateIoCompletionPort�Լ��� 2���� �뵵�� ����Ѵ�.
	// 1. IOCPĿ�� ������Ʈ�� �����ϱ����� ���ȴ�.
	//    ó���� �ѹ� ȣ���� ������Ѵ�.
	//    ȣ���Ŀ��� IOCP �ڵ��� ��ȯ�Ѵ�.
	// 2. IOCP �Ϸ� ��Ʈ �ڵ�� ���� �ڵ��� �����ϸ� ���μ������� 
	//    �ش� ���� �ڵ�� ���õ� �񵿱� I/O �۾��� �Ϸ� �˸��� ������ �ִ�.
	
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
