#include "pch.h"
#include "ReceiveBuffer.h"

ReceiveBuffer::ReceiveBuffer() : m_recvBuffer{}, m_wsaRecvieBuf{}, m_iReadPos{}, m_iWritePos{}
{

}

WSABUF* ReceiveBuffer::GetWSABuf()
{
	m_wsaRecvieBuf.buf = &m_recvBuffer[m_iWritePos];
	m_wsaRecvieBuf.len = BUF_MAX_SIZE - m_iWritePos;

	return &m_wsaRecvieBuf;
}

void ReceiveBuffer::AllReadCheck()
{
	if (m_iReadPos == m_iWritePos)
	{
		// �а��� ��ġ�� ������� �ʱ�ȭ
		m_iReadPos = m_iWritePos = 0;
		memset(m_recvBuffer, 0, BUF_MAX_SIZE);
	}
	else
	{
		// �ƴҰ�� �а� ���� ��ġ�� ������ ����.
		::memcpy(&m_recvBuffer[0], &m_recvBuffer[m_iReadPos], GetRecvUseBuffer());
		m_iReadPos = 0;
		m_iWritePos = GetRecvUseBuffer();
	}
}

void ReceiveBuffer::WritePosMove(DWORD _bytesTransferred)
{
	m_iWritePos += _bytesTransferred;
}

void ReceiveBuffer::ReadPosMove(DWORD _bytesTransferred)
{
	m_iReadPos += _bytesTransferred;
}

int ReceiveBuffer::GetRecvUseBuffer()
{
	return m_iWritePos - m_iReadPos;
}

char* ReceiveBuffer::PacketAdress()
{
	return &m_recvBuffer[m_iReadPos];
}

