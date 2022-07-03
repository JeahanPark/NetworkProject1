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
	SocketEvent* sEvent = new SocketEvent(SocketEventType::SocketEventType_Send, this);

	DWORD recvLen = 0;
	DWORD flags = 0;

	m_vecProcessSendBuffer.push_back(_sendBuffer);

	WSASend(GetSocket(), _sendBuffer->GetWSABuf(), 1, &recvLen, flags, (LPWSAOVERLAPPED)sEvent, NULL);
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
		PacketHandler::PacketHandling(this, data);


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
	SendBuffer* sendBuffer = m_vecProcessSendBuffer[0];

	m_vecProcessSendBuffer.pop_back();

	PacketData* data = (PacketData*)sendBuffer->GetSendBufferAdress();

	// ��Ŷ����
	PacketHandler::PacketHandling(this, data);

	delete data;
}
