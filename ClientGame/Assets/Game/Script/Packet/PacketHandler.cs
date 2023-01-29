using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class PacketHandler
{
    public static void PacketHandling(int _iHeaderSize, byte[] _buffer, PacketHeader _header)
    {
		string strPacket = string.Empty;
		// ��Ŷ�� ���� �Լ� ȣ�����ֱ�... �հ� �̻��ϴ�
		
		// �ΰ��ӿ��� ȣ���Ұ��

		// �κ��ϰ�쿡�� ȣ�����ش�.
		switch (_header.m_PakcetType)
		{
			case ePacketType.SToC_Chatting:
				V2ChattingPacket Chatting = Packet.BufferToPacket<V2ChattingPacket>(_buffer, _iHeaderSize);
				LobbyController.Instance.ReceiveChattingMessage(Chatting);
				break;
			case ePacketType.SToC_PacketResult:
				PacketResult Result = Packet.BufferToPacket<PacketResult>(_buffer, _iHeaderSize);
				PacketResult(Result);
				strPacket = Result.m_eResult.ToString() + ", " + Result.m_eTargetPakcetType.ToString() + ", " + Result.m_SignalType.ToString();
				break;
			case ePacketType.SToC_LoginResult:
				LoginResultPacket LoginResult = Packet.BufferToPacket<LoginResultPacket>(_buffer, _iHeaderSize);
				LobbyController.Instance.ReceiveLoginResult(LoginResult);
				break;
			case ePacketType.SToC_InGameUpdate:
				var packet = Packet.BufferToPacket<InGameUpdatePacket, InteractionPacketData>(_buffer, _iHeaderSize);
				InGameController.Instance.ReceiveInGameUpdate(packet.Item1, packet.Item2);
				break;
				//case ePacketType.STOC_InGameEnter_Success:
				//	InGameEnterSuccess inGameEnterSuccess = Packet.BufferToPacket<InGameEnterSuccess>(_buffer, _iHeaderSize);
				//	LobbyController.Instance.ReceiveInGameEnter(inGameEnterSuccess);
				//	break;
		}

		if (_header.m_PakcetType != ePacketType.SToC_PacketResult)
		{
			strPacket = _header.m_PakcetType.ToString();
		}

		//Debug.Log(strPacket);
	}

	private static void PacketResult(PacketResult _packetResult)
	{
		// ������ �ñ׳� �����ϰ��
		if (_packetResult.m_eTargetPakcetType == ePacketType.Signal &&
			_packetResult.m_SignalType != ePacketSignal.NONE)
		{
			switch (_packetResult.m_SignalType)
			{
				case ePacketSignal.Signal_ChattingRoomEnter:
				case ePacketSignal.Signal_ChattingRoomExit:
					LobbyController.Instance.ReceiveChattingRoom(_packetResult);
					break;
				case ePacketSignal.Signal_InGameExit:
					LobbyController.Instance.ReceiveInGameFaile(_packetResult);
					break;
				case ePacketSignal.Signal_InGameEnter:
					LobbyController.Instance.ReceiveInGameEnter(_packetResult);
					break;

			}
		}

		switch (_packetResult.m_eTargetPakcetType)
		{
			case ePacketType.CToS_UserRegister:
				LobbyController.Instance.ReceiveRegister(_packetResult.m_eResult);
				break;
		}
	}
}
