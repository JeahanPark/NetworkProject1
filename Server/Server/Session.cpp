#include "pch.h"
#include "Session.h"

static int SessionNumber = 0;

Session::Session()
{
	m_wsaRecvieBuf = new WSABUF();
	m_wsaSendBuf = new WSABUF();

	memset(m_recvBuffer, 0, BUFSIZE);
	memset(m_sendBuffer, 0, BUFSIZE);

	m_iSessionNumber = SessionNumber++;
}

void Session::SendChatting(char chattingContent[])
{
	SocketEvent* sEvent = new SocketEvent(SocketEventType::SocketEventType_Send, this);

	ChattingPacket chatting;
	chatting.m_PakcetType = PacketType::Both_Chatting;
	chatting.m_iSize = sizeof(ChattingPacket);
}

WSABUF* Session::GetRecvieWSABUF()
{
	//memset(m_recvBuffer, 0, BUFSIZE);

	//m_wsaRecvieBuf->buf = m_recvBuffer;
	//m_wsaRecvieBuf->len = BUFSIZE;

	m_wsaRecvieBuf->buf = &m_recvBuffer[m_iWritePos];
	m_wsaRecvieBuf->len = BUFSIZE - m_iWritePos;

	return m_wsaRecvieBuf;
}

WSABUF* Session::GetInitSendWSABUF()
{
	memset(m_sendBuffer, 0, BUFSIZE);

	m_wsaSendBuf->buf = m_sendBuffer;
	m_wsaSendBuf->len = BUFSIZE;
	return m_wsaSendBuf;
}

string Session::GetSessionNumber()
{
	string strNum = "session";

	strNum += to_string(m_iSessionNumber);

	return strNum;
}

void Session::SendPacketHandling()
{

}

void Session::ReceivePacketHandling(DWORD _bytesTransferred)
{
	// ���� �����͸� �޾Ҵ�.
	// ���� ��ġ�� �ű��.
	m_iWritePos += _bytesTransferred;

	while (true)
	{
		if (sizeof(PacketData) > GetRecvUseBuffer())
		{
			// �ּ� ��Ŷ������� �۴�.
			return;
		}

		PacketData* data = (PacketData*)&m_recvBuffer[m_iReadPos];
		if (data->m_iSize > GetRecvUseBuffer())
		{
			// �ش� ��Ŷ�� ũ�⺸�� �۴�.
			return;
		}

		// ��Ŷ ����
		PacketHandler::PacketHandling(data);


		// ���� ��Ŷ�� ����� �߰��Ѵ�.
		m_iReadPos += data->m_iSize;
	}


	
	if (m_iReadPos == m_iWritePos)
	{
		// �а��� ��ġ�� ������� �ʱ�ȭ
		m_iReadPos = m_iWritePos = 0;
		memset(m_recvBuffer, 0, BUFSIZE);
	}
	else
	{
		// �ƴҰ�� �а� ���� ��ġ�� ������ ����.
		::memcpy(&m_recvBuffer[0], &m_recvBuffer[m_iReadPos], GetRecvUseBuffer());
		m_iReadPos = 0;
		m_iWritePos = GetRecvUseBuffer();
	}
}
