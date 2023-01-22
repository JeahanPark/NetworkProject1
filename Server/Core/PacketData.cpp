#include "pch.h"
#include "PacketData.h"


SendBuffer* PacketCreate::ChattingPacketCreate(WCHAR* _content, ePacketType _ePacketType)
{
	SendBuffer* pSendBuffer = new SendBuffer(sizeof(ChattingPacket));

	ChattingPacket* chatting = (ChattingPacket*)pSendBuffer->GetSendBufferAdress();
	chatting->m_PakcetType = _ePacketType;
	chatting->m_iSize = sizeof(ChattingPacket);
	wcscpy_s(chatting->m_chattingContent, _content);

	return pSendBuffer;
}
