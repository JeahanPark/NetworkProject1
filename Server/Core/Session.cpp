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

	// 2���� ����
	// IOCP �Ϸ� ��Ʈ �ڵ�� ���� �ڵ��� �����ϸ� ���μ������� �ش� ���� �ڵ�� ���õ� �񵿱� I/O �۾��� �Ϸ� �˸��� ������ �ִ�.
	// �ڵ�(����)�� iocp�� ����
	CreateIoCompletionPort((HANDLE)m_socket, _iocpHandle, (ULONG_PTR)this, 0);

	{
		// ������ �ּҿ� ������ ���̸� ����
		// �ּҿ� ���̸� �����ϴ�����
		// WSABUF �迭 �������� �ѹ��� ���� �����ϰԲ� �ϱ�����
		// ���۸� ��Ƽ� �ѹ��� ����ϴ±���� Scatter-Gather

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

	// �����ְ� ���� �ٲ�� ���� ���� �Ѱ��ش�.
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
	// ���� �����͸� �޾Ҵ�.
	// ���� ��ġ�� �ű��.
	m_ReceiveBuffer->WritePosMove(_bytesTransferred);

	while (true)
	{
		if (sizeof(BasePacket) > m_ReceiveBuffer->GetRecvUseBuffer())
		{
			// �ּ� ��Ŷ������� �۴�.
			break;
		}
	

		BasePacket* data = (BasePacket*)(m_ReceiveBuffer->PacketAdress());
		if (data->m_iSize > m_ReceiveBuffer->GetRecvUseBuffer())
		{
			// �ش� ��Ŷ�� ũ�⺸�� �۴�.
			break;
		}

		// ��Ŷ ����
		// 
		//PacketHandler::PacketHandling(this, data);
		PacketeHandle(data);

		// ���� ��Ŷ�� ����� �߰��Ѵ�.
		m_ReceiveBuffer->ReadPosMove(data->m_iSize);
	}

	// �����͸� ��� �о������
	m_ReceiveBuffer->AllReadCheck();

	// ���ú긦 �ٽ� �������ֵ��� ��û����
	RegisterReceive();
}

void Session::ProcessSend(DWORD _bytesTransferred)
{
	if (m_RegisterDisconnect)
		return;

	for (SendBuffer* sendBuffer : m_lisProcessSendBuffer)
	{
		BasePacket* data = (BasePacket*)sendBuffer->GetSendBufferAdress();

		// ��Ŷ����
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

	// �׿��ִ� Send���� �ѹ��� �Ѱ��ش�.
	if (SOCKET_ERROR == WSASend(GetSocket(), vecWSABUF.data(), (DWORD)vecWSABUF.size(), &recvLen, flags, (LPWSAOVERLAPPED)sEvent, NULL))
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			SocketEventError(errorCode);
		}
	}
}