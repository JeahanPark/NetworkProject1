#include "pch.h"
#include "ReceiveBuffer.h"

ReceiveBuffer::ReceiveBuffer()
{
	memset(m_recvBuffer, 0, BUFSIZE);
}

WSABUF* ReceiveBuffer::GetWSABuf()
{
	m_wsaRecvieBuf.buf = &m_recvBuffer[m_iWritePos];
	m_wsaRecvieBuf.len = BUFSIZE - m_iWritePos;

	return &m_wsaRecvieBuf;
}

void ReceiveBuffer::AllReadCheck()
{
	if (m_iReadPos == m_iWritePos)
	{
		// 읽고쓰는 위치가 같을경우 초기화
		m_iReadPos = m_iWritePos = 0;
		memset(m_recvBuffer, 0, BUFSIZE);
	}
	else
	{
		// 아닐경우 읽고 쓰는 위치를 앞으로 당긴다.
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

char& ReceiveBuffer::PacketAdress()
{
	return m_recvBuffer[m_iReadPos];
}

