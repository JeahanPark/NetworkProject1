using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System;
public enum PacketType
{
	NONE = 0,
	// 서버에서 클라로

	SToC_Login,
	SToC_Chatting,

	// 서버에서 클라로

	// 클라에서 서버로

	CToS_Login,
	CToS_Chatting,

	// 클라에서 서버로

	END,
}

public class PacketData
{
	public PacketType m_PakcetType;
	public int m_iSize;
};

public class LoginPacket : PacketData
{

};


public class ChattingPacket : PacketData
{
	char[] chattingContent;
};

// 아... 나눌껄....
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct PacketHeader
{
	public PacketType m_PakcetType;
	public int m_iSize;
}

// 멤버변수들을 메모리 바운더리 상에 정렬하는 Field Alignment 규칙에 따른 것이다.
// 즉, 아래의 구조체의 경우 8바이트 바운더리에 맞춘 것이다.
// 이러한 Alignment는 C# / .NET에서 자동으로 처리되는데,
// 경우에 따라 개발자가 이를 변경할 필요가 있을 때가 있다.
// 그것이 StructLayout 이거 Pack=1은 1바이트로 정렬
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct V2ChattingPacket
{
    public PacketType m_PakcetType;
    public int m_iSize;
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 500)]
	public string chattingContent;
}



public class Packet
{
	private static T BufferToPacket<T>(byte[] _buffer) where T : struct
    {
		int size = Marshal.SizeOf(typeof(T));
		IntPtr ptr = Marshal.AllocHGlobal(size);
		Marshal.Copy(_buffer, 0, ptr, size);
		T packet = (T)Marshal.PtrToStructure(ptr, typeof(T));
		Marshal.FreeHGlobal(ptr);

		return packet;
	}

	public static bool ReceviePacketHandle(byte[] _buffer, int _iBufferSize)
    {
        // 패킷 헤더 사이즈 체크
        {
			int iHearSize = Marshal.SizeOf(typeof(PacketHeader));
			if (_iBufferSize < iHearSize)
				return false;
		}

		// 패킷 사이즈가 부족하다.
		PacketHeader packetHeader;
		{
			packetHeader = BufferToPacket<PacketHeader>(_buffer);
			if (packetHeader.m_iSize < _iBufferSize)
				return false;
		}

		// 패킷에 따라 함수 호출해주기... 먼가 이상하다
        {
			switch (packetHeader.m_PakcetType)
			{
				case PacketType.SToC_Chatting:
					V2ChattingPacket chatting = BufferToPacket<V2ChattingPacket>(_buffer);
					LobbyController.Instance.ReceiveChattingMessage(chatting);
					break;
			}
		}

		return true;
	}

	public static void Chatting(string _strText)
    {
		V2ChattingPacket chattingPacket = new V2ChattingPacket();
		chattingPacket.m_PakcetType = PacketType.CToS_Chatting;
		chattingPacket.chattingContent = _strText;

		// SizeOf 사용하여 할당할 관리되지 않는 메모리의 양을 결정
		int size = Marshal.SizeOf(chattingPacket);

		chattingPacket.m_iSize = size;

		byte[] arr = new byte[size];

		// 프로세스의 관리되지 않는 메모리에서 메모리를 할당합니다.
		IntPtr ptr = Marshal.AllocHGlobal(size);

		// 관리되는 개체의 데이터를 관리되지 않는 메모리 블록으로 마샬링합니다.
		Marshal.StructureToPtr(chattingPacket, ptr, true);
		Marshal.Copy(ptr, arr, 0, size);

		// 프로세스의 할당한 관리되지 않는 메모리를 해제시킵니다.
		Marshal.FreeHGlobal(ptr);

		SocketManager.Instance.Send(arr);
	}
}