#include "pch.h"
#include "Session.h"
#include "ReceiveBuffer.h"
#include "SendBuffer.h"

static int SessionNumber = 0;

Session::Session()
{
	m_ReceiveBuffer = new ReceiveBuffer();
	m_iSessionNumber = SessionNumber++;
}

Session::~Session()
{
	delete m_ReceiveBuffer;
}


void Session::InitSession(HANDLE _iocpHandle, SOCKET _socket)
{
	m_Socket = _socket;

	cout << GetSessionNumber() << " Join!!!" << endl;

	// 2번의 형식
	// IOCP 완료 포트 핸들과 소켓 핸들을 연결하면 프로세스에서 해당 소켓 핸들과 관련된 비동기 I/O 작업의 완료 알림을 받을수 있다.
	// 핸들(소켓)을 iocp와 연결
	CreateIoCompletionPort((HANDLE)m_Socket, _iocpHandle, (ULONG_PTR)this, 0);

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

void Session::RegisterReceive()
{
	SocketEvent* sEvent = new SocketEvent(SocketEventType::SocketEventType_Receive, this);

	DWORD recvLen = 0;
	DWORD flags = 0;

	WSARecv(GetSocket(), m_ReceiveBuffer->GetWSABuf(), 1, &recvLen, &flags, (LPWSAOVERLAPPED)sEvent, NULL);
}

void Session::RegisterSend(SendBuffer* _sendBuffer)
{
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

void Session::ProcessReceive(DWORD _bytesTransferred)
{
	// 새로 데이터를 받았다.
	// 쓰는 위치를 옮긴다.
	m_ReceiveBuffer->WritePosMove(_bytesTransferred);

	while (true)
	{
		if (sizeof(PacketData) > m_ReceiveBuffer->GetRecvUseBuffer())
		{
			// 최소 패킷헤더보다 작다.
			break;
		}
	

		PacketData* data = (PacketData*)(m_ReceiveBuffer->PacketAdress());
		if (data->m_iSize > m_ReceiveBuffer->GetRecvUseBuffer())
		{
			// 해당 패킷의 크기보다 작다.
			break;
		}

		// 패킷 조립
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
	for (SendBuffer* sendBuffer : m_lisProcessSendBuffer)
	{
		PacketData* data = (PacketData*)sendBuffer->GetSendBufferAdress();

		// 패킷조립
		//PacketHandler::PacketHandling(this, data);

		delete data;
	}

	m_lisProcessSendBuffer.clear();

	LockGuard lock(m_lockSending);
	
	if (m_lisRegisterSendBuffer.empty())
		m_RegisterSendBufferAdd.store(false);
	else
		Send();
}

void Session::Send()
{
	vector<WSABUF> vecWSABUF;
	{
		LockGuard lock(m_lockSending);

		m_lisProcessSendBuffer.assign(m_lisRegisterSendBuffer.begin(), m_lisRegisterSendBuffer.end());

		m_lisRegisterSendBuffer.clear();


		vecWSABUF.reserve(m_lisProcessSendBuffer.size());

		for (SendBuffer* sendBuffer : m_lisProcessSendBuffer)
		{
			vecWSABUF.push_back(*sendBuffer->GetWSABuf());
		}
	}
	
	SocketEvent* sEvent = new SocketEvent(SocketEventType::SocketEventType_Send, this);

	DWORD recvLen = 0;
	DWORD flags = 0;

	// 쌓여있는 Send들을 한번에 넘겨준다.
	WSASend(GetSocket(), vecWSABUF.data(), (DWORD)vecWSABUF.size(), &recvLen, flags, (LPWSAOVERLAPPED)sEvent, NULL);
}