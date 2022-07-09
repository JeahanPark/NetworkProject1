#include "pch.h"
#include "PacketData.h"


SendBuffer* PacketCreate::ChattingPacketCreate(char* _content, PacketType _ePacketType)
{
	SendBuffer* pSendBuffer = new SendBuffer(sizeof(ChattingPacket));

	ChattingPacket* chatting = (ChattingPacket*)pSendBuffer->GetSendBufferAdress();
	chatting->m_PakcetType = _ePacketType;
	chatting->m_iSize = sizeof(ChattingPacket);
	strcpy_s(chatting->chattingContent, _content);

	pSendBuffer->WsaBufSetting();

	return pSendBuffer;
}
