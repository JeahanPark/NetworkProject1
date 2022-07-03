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
		PacketHandler::PacketHandling(this, data);


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
	SendBuffer* sendBuffer = m_vecProcessSendBuffer[0];

	m_vecProcessSendBuffer.pop_back();

	PacketData* data = (PacketData*)sendBuffer->GetSendBufferAdress();

	// 패킷조립
	PacketHandler::PacketHandling(this, data);

	delete data;
}
