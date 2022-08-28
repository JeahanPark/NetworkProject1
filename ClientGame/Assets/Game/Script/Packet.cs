using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System;
public enum ePacketType
{
	NONE = 0,
	// 서버에서 클라로

	SToC_Login,
	SToC_Chatting,
	SToC_PacketResult,
	// 서버에서 클라로

	// 클라에서 서버로

	CToS_Login,
	CToS_Chatting,
	CToS_UserRegister,
	// 클라에서 서버로

	END,
}

public enum ePacketResult
{
	Success,
};

// 아... 나눌껄....
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct PacketHeader
{
	public ePacketType m_PakcetType;
	public int m_iSize;
}

// 멤버변수들을 메모리 바운더리 상에 정렬하는 Field Alignment 규칙에 따른 것이다.
// 즉, 아래의 구조체의 경우 8바이트 바운더리에 맞춘 것이다.
// 이러한 Alignment는 C# / .NET에서 자동으로 처리되는데,
// 경우에 따라 개발자가 이를 변경할 필요가 있을 때가 있다.
// 그것이 StructLayout 이거 Pack=1은 1바이트로 정렬

// LayoutKind.Sequential = 순서대로 정렬한다. 원래는 지멋대로 정렬함
// Pack = 멤버변수 사이즈를 최소를 1바이트로 잡아둠
// CharSet = 샬링된 문자열이 사용할 문자 집합을 지정합니다.
[StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Unicode)]
public struct V2ChattingPacket
{
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 300)]
	public string chattingContent;
}


[StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Unicode)]
public struct UserRegistPacket
{
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_UserID;
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_Password;

	public int Score;
};

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct PacketResult
{
	public ePacketType		m_eTargetPakcetType;
	public ePacketResult	m_eResult;
};

public class Packet
{
	private static T BufferToPacket<T>(byte[] _buffer, int _startIndex) where T : struct
    {
		int size = Marshal.SizeOf(typeof(T));
		IntPtr ptr = Marshal.AllocHGlobal(size);
		Marshal.Copy(_buffer, _startIndex, ptr, size);
		T packet = (T)Marshal.PtrToStructure(ptr, typeof(T));
		Marshal.FreeHGlobal(ptr);

		return packet;
	}

	public static bool ReceviePacketHandle(byte[] _buffer, int _iBufferSize)
    {
		int iHearSize = 0;
		// 패킷 헤더 사이즈 체크
		{
			iHearSize = Marshal.SizeOf(typeof(PacketHeader));
			if (_iBufferSize < iHearSize)
				return false;
		}

		// 패킷 사이즈가 부족하다.
		PacketHeader packetHeader;
		{
			packetHeader = BufferToPacket<PacketHeader>(_buffer, 0);
			if (packetHeader.m_iSize < _iBufferSize)
				return false;
		}

		// 패킷에 따라 함수 호출해주기... 먼가 이상하다
        {
			switch (packetHeader.m_PakcetType)
			{
				case ePacketType.SToC_Chatting:
					V2ChattingPacket Chatting = BufferToPacket<V2ChattingPacket>(_buffer, iHearSize);
					LobbyController.Instance.ReceiveChattingMessage(Chatting);
					break;
				case ePacketType.SToC_PacketResult:
					PacketResult Result = BufferToPacket<PacketResult>(_buffer, iHearSize);
					PacketResult(Result);
					break;
			}
		}

		return true;
	}

	public static void PacketResult(PacketResult _packetResult)
    {
        switch (_packetResult.m_eTargetPakcetType)
        {
			case ePacketType.CToS_UserRegister:
				LobbyController.Instance.ReceiveRegister(_packetResult.m_eResult);
				break;
		}
    }
    public static void SendPacket<T>(T _packet, ePacketType packetType) where T : struct
    {
		PacketHeader packetHeader = new PacketHeader();
		packetHeader.m_PakcetType = packetType;

		// SizeOf 사용하여 할당할 관리되지 않는 메모리의 양을 결정
		int headerSize = Marshal.SizeOf(packetHeader);
		int BodySize = Marshal.SizeOf(_packet);

		packetHeader.m_iSize = headerSize + BodySize;

		byte[] arr = new byte[packetHeader.m_iSize];

		// 프로세스의 관리되지 않는 메모리에서 메모리를 할당합니다.
		IntPtr headerPtr = Marshal.AllocHGlobal(headerSize);
		IntPtr bodyPtr = Marshal.AllocHGlobal(BodySize);

		// 관리되는 개체의 데이터를 관리되지 않는 메모리 블록으로 마샬링합니다.

		Marshal.StructureToPtr(packetHeader, headerPtr, true);
		Marshal.StructureToPtr(_packet, bodyPtr, true);

		Marshal.Copy(headerPtr, arr, 0, headerSize);
		Marshal.Copy(bodyPtr, arr, headerSize, BodySize);

		// 프로세스의 할당한 관리되지 않는 메모리를 해제시킵니다.
		Marshal.FreeHGlobal(headerPtr);
		Marshal.FreeHGlobal(bodyPtr);

		SocketManager.Instance.Send(arr);
	}
}