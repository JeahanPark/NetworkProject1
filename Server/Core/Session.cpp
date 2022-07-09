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

	// 2���� ����
	// IOCP �Ϸ� ��Ʈ �ڵ�� ���� �ڵ��� �����ϸ� ���μ������� �ش� ���� �ڵ�� ���õ� �񵿱� I/O �۾��� �Ϸ� �˸��� ������ �ִ�.
	// �ڵ�(����)�� iocp�� ����
	CreateIoCompletionPort((HANDLE)m_Socket, _iocpHandle, (ULONG_PTR)this, 0);

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

	// �����ְ� ���� �ٲ�� ���� ���� �Ѱ��ش�.
	if (m_RegisterSendBufferAdd.exchange(true) == false)
		sendRequast = true;

	if (sendRequast)
		Send();
}

void Session::ProcessReceive(DWORD _bytesTransferred)
{
	// ���� �����͸� �޾Ҵ�.
	// ���� ��ġ�� �ű��.
	m_ReceiveBuffer->WritePosMove(_bytesTransferred);

	while (true)
	{
		if (sizeof(PacketData) > m_ReceiveBuffer->GetRecvUseBuffer())
		{
			// �ּ� ��Ŷ������� �۴�.
			break;
		}
	

		PacketData* data = (PacketData*)(m_ReceiveBuffer->PacketAdress());
		if (data->m_iSize > m_ReceiveBuffer->GetRecvUseBuffer())
		{
			// �ش� ��Ŷ�� ũ�⺸�� �۴�.
			break;
		}

		// ��Ŷ ����
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
	for (SendBuffer* sendBuffer : m_lisProcessSendBuffer)
	{
		PacketData* data = (PacketData*)sendBuffer->GetSendBufferAdress();

		// ��Ŷ����
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

	// �׿��ִ� Send���� �ѹ��� �Ѱ��ش�.
	WSASend(GetSocket(), vecWSABUF.data(), (DWORD)vecWSABUF.size(), &recvLen, flags, (LPWSAOVERLAPPED)sEvent, NULL);
}