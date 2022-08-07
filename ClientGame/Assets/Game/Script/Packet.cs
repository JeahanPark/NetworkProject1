using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System;
public enum PacketType
{
	NONE = 0,
	// �������� Ŭ���

	SToC_Login,
	SToC_Chatting,

	// �������� Ŭ���

	// Ŭ�󿡼� ������

	CToS_Login,
	CToS_Chatting,

	// Ŭ�󿡼� ������

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

// ��... ������....
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct PacketHeader
{
	public PacketType m_PakcetType;
	public int m_iSize;
}

// ����������� �޸� �ٿ���� �� �����ϴ� Field Alignment ��Ģ�� ���� ���̴�.
// ��, �Ʒ��� ����ü�� ��� 8����Ʈ �ٿ������ ���� ���̴�.
// �̷��� Alignment�� C# / .NET���� �ڵ����� ó���Ǵµ�,
// ��쿡 ���� �����ڰ� �̸� ������ �ʿ䰡 ���� ���� �ִ�.
// �װ��� StructLayout �̰� Pack=1�� 1����Ʈ�� ����
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
        // ��Ŷ ��� ������ üũ
        {
			int iHearSize = Marshal.SizeOf(typeof(PacketHeader));
			if (_iBufferSize < iHearSize)
				return false;
		}

		// ��Ŷ ����� �����ϴ�.
		PacketHeader packetHeader;
		{
			packetHeader = BufferToPacket<PacketHeader>(_buffer);
			if (packetHeader.m_iSize < _iBufferSize)
				return false;
		}

		// ��Ŷ�� ���� �Լ� ȣ�����ֱ�... �հ� �̻��ϴ�
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

		// SizeOf ����Ͽ� �Ҵ��� �������� �ʴ� �޸��� ���� ����
		int size = Marshal.SizeOf(chattingPacket);

		chattingPacket.m_iSize = size;

		byte[] arr = new byte[size];

		// ���μ����� �������� �ʴ� �޸𸮿��� �޸𸮸� �Ҵ��մϴ�.
		IntPtr ptr = Marshal.AllocHGlobal(size);

		// �����Ǵ� ��ü�� �����͸� �������� �ʴ� �޸� ������� �������մϴ�.
		Marshal.StructureToPtr(chattingPacket, ptr, true);
		Marshal.Copy(ptr, arr, 0, size);

		// ���μ����� �Ҵ��� �������� �ʴ� �޸𸮸� ������ŵ�ϴ�.
		Marshal.FreeHGlobal(ptr);

		SocketManager.Instance.Send(arr);
	}
}