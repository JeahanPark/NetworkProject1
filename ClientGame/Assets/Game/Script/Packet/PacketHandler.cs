using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class PacketHandler
{
    public static void PacketHandling(int _iHearSize, byte[] _buffer, PacketHeader _header)
    {
		string strPacket = string.Empty;
		// 패킷에 따라 함수 호출해주기... 먼가 이상하다
		
		// 인게임에서 호출할경우
		if (SceneManager.Instance.GetCurentScene() == SceneType.InGame)
		{
			switch(_header.m_PakcetType)
            {
				case ePacketType.SToC_InGameUpdate:
					InGameUpdatePacket packet = Packet.BufferToPacket<InGameUpdatePacket>(_buffer, _iHearSize);
					InGameController.Instance.ReceiveInGameUpdate(packet);
					break;
            }
		}

		// 로비일경우에만 호출해준다.
		if (SceneManager.Instance.GetCurentScene() == SceneType.Lobby)
		{
			switch (_header.m_PakcetType)
			{
				case ePacketType.SToC_Chatting:
					V2ChattingPacket Chatting = Packet.BufferToPacket<V2ChattingPacket>(_buffer, _iHearSize);
					LobbyController.Instance.ReceiveChattingMessage(Chatting);
					break;
				case ePacketType.SToC_PacketResult:
					PacketResult Result = Packet.BufferToPacket<PacketResult>(_buffer, _iHearSize);
					PacketResult(Result);
					strPacket = Result.m_eResult.ToString() + ", " + Result.m_eTargetPakcetType.ToString() + ", " + Result.m_SignalType.ToString();
					break;
				case ePacketType.SToC_LoginResult:
					LoginResultPacket LoginResult = Packet.BufferToPacket<LoginResultPacket>(_buffer, _iHearSize);
					LobbyController.Instance.ReceiveLoginResult(LoginResult);
					break;
				case ePacketType.STOC_InGameEnter_Success:
					InGameEnterSuccess inGameEnterSuccess = Packet.BufferToPacket<InGameEnterSuccess>(_buffer, _iHearSize);
					LobbyController.Instance.ReceiveInGameEnter(inGameEnterSuccess);
					break;
			}
		}

		if (_header.m_PakcetType != ePacketType.SToC_PacketResult)
		{
			strPacket = _header.m_PakcetType.ToString();
		}

		//Debug.Log(strPacket);
	}

	private static void PacketResult(PacketResult _packetResult)
	{
		// 간단한 시그널 응답일경우
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
