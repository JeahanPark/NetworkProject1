using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class PacketHandler
{
    public static void PacketHandling(int _iHeaderSize, byte[] _buffer, PacketHeader _header)
    {
		string strPacket = string.Empty;
		// 패킷에 따라 함수 호출해주기... 먼가 이상하다
		
		// 인게임에서 호출할경우

		// 로비일경우에만 호출해준다.
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
                {
					var packet = Packet.BufferToPacket<InGameUpdatePacket, InteractionData>(_buffer, _iHeaderSize);
					InGameController.Instance.ReceiveInGameUpdate(packet.Item1, packet.Item2);
				}
				break;
			case ePacketType.SToC_NewUserInteraction:
                {
					var packet = Packet.BufferToPacket<NewUserPacket>(_buffer, _iHeaderSize);
                    InGameController.Instance.ReceiveNewUser(packet);
                }
                break;
			case ePacketType.SToC_InitialInGameData:
                {
					var packet = Packet.BufferToPacket<InitialInGameDataPacket, InitialInGameData>(_buffer, _iHeaderSize);
					InGameController.Instance.ReceiveInitialInGameData(packet.Item1, packet.Item2);
				}
				break;
			case ePacketType.SToC_RecivedDamage:
				{
					var packet = Packet.BufferToPacket<RecivedDamagePacket>(_buffer, _iHeaderSize);
					InGameController.Instance.RecivedDamage(packet);
				}
				break;
			case ePacketType.SToC_UserRiseAgain:
                {
					var packet = Packet.BufferToPacket<UserRiseAgainPacket>(_buffer, _iHeaderSize);
					InGameController.Instance.ReciveUserRiseAgain(packet);
				}
				break;
			case ePacketType.SToC_UpdatetMySkill:
                {
					var packet = Packet.BufferToPacket<UpdatetMySkillPacket>(_buffer, _iHeaderSize);
					InGameController.Instance.ReciveUpdatetMySkill(packet);
				}
				break;
			case ePacketType.SToC_UpdatePoint:
				{
					var packet = Packet.BufferToPacket<UpdatePointPacket>(_buffer, _iHeaderSize);
					InGameController.Instance.ReciveUpdatePoint(packet);
				}
				break;
			case ePacketType.SToC_ShowSkill:
				{
					var packet = Packet.BufferToPacket<ShowSkillPacket>(_buffer, _iHeaderSize);
					InGameController.Instance.ReciveShowSkill(packet);
				}
				break;
			case ePacketType.SToC_ShowReflectionEffect:
                {
					var packet = Packet.BufferToPacket<ShowReflectionEffectPacket>(_buffer, _iHeaderSize);
					InGameController.Instance.ReciveShowReflectionEffect(packet);
				}
				break;
		}

		if (_header.m_PakcetType != ePacketType.SToC_PacketResult)
		{
			strPacket = _header.m_PakcetType.ToString();
		}

		Debug.Log(strPacket);
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
					LobbyController.Instance.ReceiveInGameFail(_packetResult);
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
