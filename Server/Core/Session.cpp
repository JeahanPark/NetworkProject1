#include "pch.h"
#include "Session.h"
#include "ReceiveBuffer.h"
#include "SendBuffer.h"

static int SessionNumber = 0;

Session::Session() : m_ReceiveBuffer(new ReceiveBuffer()), m_iSessionNumber(SessionNumber++), m_bSessionDisconnect(true)
{
}

Session::~Session()
{
	delete m_ReceiveBuffer;
	SocketUtil::SocketClose(m_socket);
}


void Session::InitSession(HANDLE _iocpHandle, SOCKET _socket)
{
	m_socket = _socket;

	cout << GetSessionNumber() << " Connect!!!" << endl;

	// 2번의 형식
	// IOCP 완료 포트 핸들과 소켓 핸들을 연결하면 프로세스에서 해당 소켓 핸들과 관련된 비동기 I/O 작업의 완료 알림을 받을수 있다.
	// 핸들(소켓)을 iocp와 연결
	CreateIoCompletionPort((HANDLE)m_socket, _iocpHandle, (ULONG_PTR)this, 0);

	{
		// 버퍼의 주소와 버퍼의 길이를 저장
		// 주소와 길이를 저장하는이유
		// WSABUF 배열 형식으로 한번에 전달 가능하게끔 하기위해
		// 버퍼를 모아서 한번에 출력하는기법이 Scatter-Gather

		RegisterReceive();
	}
}

string Session::GetSessionNumber()
{
	string strNum = "session";

	strNum += to_string(m_iSessionNumber);

	return strNum;
}

bool Session::IsSessionConnect()
{
	return !m_bSessionDisconnect;
}

void Session::RegisterReceive()
{
	if (m_RegisterDisconnect)
		return;

	SocketEvent* sEvent = new SocketEvent(SocketEventType::SocketEventType_Receive, shared_from_this());

	DWORD recvLen = 0;
	DWORD flags = 0;

	if (SOCKET_ERROR == WSARecv(GetSocket(), m_ReceiveBuffer->GetWSABuf(), 1, &recvLen, &flags, (LPWSAOVERLAPPED)sEvent, NULL))
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			SocketEventError(errorCode);
		}
	}
}

void Session::RegisterSend(SendBuffer* _sendBuffer)
{
	if (m_RegisterDisconnect)
		return;

	if (m_iSessionNumber == 2)
	{
		int a = 0;
	}

	_sendBuffer->WsaBufSetting();

	{
		LockGuard lock(m_lockSending);

		m_lisRegisterSendBuffer.push_back(_sendBuffer);
	}

	bool sendRequast = false;

	// 값을넣고 값이 바뀌기 전의 값을 넘겨준다.
	if (m_RegisterSendBufferAdd.exchange(true) == false)
		sendRequast = true;

	if (sendRequast)
		Send();
}

void Session::RegisterDisconnect()
{
	if (m_RegisterDisconnect.exchange(true) == true)
		return;

	SocketEvent* sEvent = new SocketEvent(SocketEventType::SocketEventType_Disconnect, shared_from_this());
	if (SOCKET_ERROR == SocketUtil::m_gDisconnect(m_socket, (LPWSAOVERLAPPED)sEvent, TF_REUSE_SOCKET, 0))
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			SocketEventError(error);
			CRASH("Disconnect Fail");
		}
	}
}

void Session::ProcessReceive(DWORD _bytesTransferred)
{
	if (m_RegisterDisconnect)
		return;

	if (_bytesTransferred <= 0)
	{
		cout << "_bytesTransferred = " << _bytesTransferred << endl;
		RegisterDisconnect();
		return;
	}
	// 새로 데이터를 받았다.
	// 쓰는 위치를 옮긴다.
	m_ReceiveBuffer->WritePosMove(_bytesTransferred);

	while (true)
	{
		if (sizeof(BasePacket) > m_ReceiveBuffer->GetRecvUseBuffer())
		{
			// 최소 패킷헤더보다 작다.
			break;
		}
	

		BasePacket* data = (BasePacket*)(m_ReceiveBuffer->PacketAdress());
		if (data->m_iSize > m_ReceiveBuffer->GetRecvUseBuffer())
		{
			// 해당 패킷의 크기보다 작다.
			break;
		}

		// 패킷 조립
		// 
		//PacketHandler::PacketHandling(this, data);
		PacketeHandle(data);

		// 읽은 패킷의 사이즈를 추가한다.
		m_ReceiveBuffer->ReadPosMove(data->m_iSize);
	}

	// 데이터를 모두 읽었을경우
	m_ReceiveBuffer->AllReadCheck();

	// 리시브를 다시 받을수있도록 요청하자
	RegisterReceive();
}

void Session::ProcessSend(DWORD _bytesTransferred)
{
	if (m_RegisterDisconnect)
		return;

	for (SendBuffer* sendBuffer : m_lisProcessSendBuffer)
	{
		BasePacket* data = (BasePacket*)sendBuffer->GetSendBufferAdress();

		// 패킷조립
		PacketeHandle(data);

		delete data;
	}

	m_lisProcessSendBuffer.clear();

	if (m_lisRegisterSendBuffer.empty())
		m_RegisterSendBufferAdd.store(false);
	else
		Send();
}

void Session::ProcessDisconnect()
{
	cout << GetSessionNumber() << " Disconnect" << endl;
	m_bSessionDisconnect = true;
	DeleteSession();
}

void Session::SocketEventError(int _iCode)
{
	cout << GetSessionNumber() << ", SocketError : " << _iCode << endl;
}


void Session::Send()
{
	int size = 0;
	vector<WSABUF> vecWSABUF;
	{
		LockGuard lock(m_lockSending);

		m_lisProcessSendBuffer.assign(m_lisRegisterSendBuffer.begin(), m_lisRegisterSendBuffer.end());

		m_lisRegisterSendBuffer.clear();


		vecWSABUF.reserve(m_lisProcessSendBuffer.size());

		for (SendBuffer* sendBuffer : m_lisProcessSendBuffer)
		{
			vecWSABUF.push_back(*sendBuffer->GetWSABuf());
			size += sendBuffer->GetWSABuf()->len;
		}
	}

	if (size >= BUF_MAX_SIZE)
	{
		cout << "BufferMaxSizeOver" << endl;
	}

	SocketEvent* sEvent = new SocketEvent(SocketEventType::SocketEventType_Send, shared_from_this());

	DWORD recvLen = 0;
	DWORD flags = 0;

	// 쌓여있는 Send들을 한번에 넘겨준다.
	if (SOCKET_ERROR == WSASend(GetSocket(), vecWSABUF.data(), (DWORD)vecWSABUF.size(), &recvLen, flags, (LPWSAOVERLAPPED)sEvent, NULL))
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			SocketEventError(errorCode);
		}
	}
}