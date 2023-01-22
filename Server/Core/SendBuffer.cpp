#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(int _BufferSize) : m_BufferSize(_BufferSize), m_sendBuffer{new char[_BufferSize]}, m_wsaSendBuf{}
{
}

SendBuffer::~SendBuffer()
{
	delete m_sendBuffer;
}

WSABUF* SendBuffer::GetWSABuf()
{
	return &m_wsaSendBuf;
}

char* SendBuffer::GetSendBufferAdress(int _startBuffer)
{
	return &m_sendBuffer[_startBuffer];
}

void SendBuffer::WsaBufSetting()
{
	m_wsaSendBuf.buf = m_sendBuffer;
	m_wsaSendBuf.len = m_BufferSize;
}
