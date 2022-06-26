#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer()
{
	memset(m_sendBuffer, 0, BUFSIZE);
}

SendBuffer::~SendBuffer()
{
}

WSABUF* SendBuffer::GetWSABuf()
{
	return &m_wsaSendBuf;
}

char& SendBuffer::GetSendBufferAdress()
{
	return m_sendBuffer[0];
}