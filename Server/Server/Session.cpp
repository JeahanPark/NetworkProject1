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

WSABUF* Session::GetInitRecvieWSABUF()
{
	memset(m_recvBuffer, 0, BUFSIZE);

	m_wsaRecvieBuf->buf = m_recvBuffer;
	m_wsaRecvieBuf->len = BUFSIZE;
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
