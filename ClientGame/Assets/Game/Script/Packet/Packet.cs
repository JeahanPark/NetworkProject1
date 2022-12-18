using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System;
using UnityEngine;

public enum ePacketType
{
	NONE = 0,

	// ��� ������ ����
	Result,

	// �����ʹ� �Ⱥ����� �����ϰ� ��ȣ�� ������� �̰ɷ� ����
	Signal,


	// �������� Ŭ���

	SToC_Chatting,
	SToC_PacketResult,
	SToC_LoginResult,
	SToC_InGameUpdate,
	STOC_InGameEnter_Success,
	// �������� Ŭ���

	// Ŭ�󿡼� ������

	CToS_Login,
	CToS_Chatting,
	CToS_UserRegister,
	CToS_MyUserMove,
	// Ŭ�󿡼� ������

	END,
}

// ����� ���� enum
public enum ePacketResult
{
	NONE = 0,
	Success,
	Fail,

	// ä�ù� ���
	ChattingRoomEnter_Not_Login,
	ChattingRoomEnter_Already_In,
	ChattingRoomExit_Not_Exist,

	// �ΰ��� ���� �����⿡ ���� ���
	InGameEnter_Not_Login,
	InGameEnter_Already_In,
	InGameEnter_InChattingRoom,
	InGameExit_Not_Exist,
};

// �����ʹ� �Ⱥ����� �����ϰ� ��ȣ�� ������� �̰ɷ� ����
public enum ePacketSignal
{
	NONE = 0,

	// ä�ù� ���
	Signal_ChattingRoomEnter,
	Signal_ChattingRoomExit,
	Signal_ChattingRoomExit_Not_Exist,

	// �ΰ��� ����, ������ ���
	Signal_InGameEnter,
	Signal_InGameExit,
};

// ��... ������....
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct PacketHeader
{
	public ePacketType m_PakcetType;
	public int m_iSize;
}

// ����������� �޸� �ٿ���� �� �����ϴ� Field Alignment ��Ģ�� ���� ���̴�.
// ��, �Ʒ��� ����ü�� ��� 8����Ʈ �ٿ������ ���� ���̴�.
// �̷��� Alignment�� C# / .NET���� �ڵ����� ó���Ǵµ�,
// ��쿡 ���� �����ڰ� �̸� ������ �ʿ䰡 ���� ���� �ִ�.
// �װ��� StructLayout �̰� Pack=1�� 1����Ʈ�� ����

// LayoutKind.Sequential = ������� �����Ѵ�. ������ ���ڴ�� ������
// Pack = ������� ����� �ּҸ� 1����Ʈ�� ��Ƶ�
// CharSet = ������ ���ڿ��� ����� ���� ������ �����մϴ�.
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
public struct V2ChattingPacket
{
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
	public string chattingContent;
}


[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
public struct UserRegistPacket
{
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_UserID;
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_Password;

	public int Score;
};

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
public struct InGameEnterPacket
{
	public int m_UserIndex;
};

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
public struct LogInPacket
{
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_UserID;
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_Password;
};
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
public struct LoginResultPacket
{
	public ePacketType m_eTargetPakcetType;
	public ePacketResult m_eResult;

	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_UserID;
	public int Score;
	public int m_iUserIndex;
};

[StructLayout(LayoutKind.Sequential)]
public struct PacketResult
{
	public ePacketType		m_eTargetPakcetType;
	public ePacketResult	m_eResult;

	public ePacketSignal	m_SignalType;
};

[StructLayout(LayoutKind.Sequential)]
public struct SignalPacket
{
	public ePacketSignal m_ePacketSignal;
};

[StructLayout(LayoutKind.Sequential)]
public struct InteractionPacketData
{
	public int							m_iInteractionIndex;
	public eInteractionType				m_eType;
	public bool							m_ValidLife;
	public PacketVector3				m_vPos;
};

[StructLayout(LayoutKind.Sequential)]
public struct InGameUpdatePacket
{
	public int m_iInteractionCount;

	[MarshalAs(UnmanagedType.ByValArray, SizeConst = EnumType.maxInteractionCount)]
	public InteractionPacketData[] m_arrInteraction;
};

[StructLayout(LayoutKind.Sequential)]
public struct PacketVector3
{
	public float x;
	public float y;
	public float z;

	public Vector3 GetVec3()
    {
		return new Vector3(x, y, z);
    }
}

[StructLayout(LayoutKind.Sequential)]
public struct InGameEnterSuccess
{
	public int m_iInteractionIndex;
	public eInteractionType m_eType;
};
public class Packet
{
	public static T BufferToPacket<T>(byte[] _buffer, int _startIndex) where T : struct
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
		// ��Ŷ ��� ������ üũ
		{
			iHearSize = Marshal.SizeOf(typeof(PacketHeader));
			if (_iBufferSize < iHearSize)
				return false;
		}

		// ��Ŷ ����� �����ϴ�.
		PacketHeader packetHeader;
		{
			packetHeader = BufferToPacket<PacketHeader>(_buffer, 0);
			if (packetHeader.m_iSize < _iBufferSize)
				return false;
		}

		// ��Ŷ 
		PacketHandler.PacketHandling(iHearSize, _buffer, packetHeader);
		
		return true;
	}


    public static void SendPacket<T>(T _packet, ePacketType packetType) where T : struct
    {
		PacketHeader packetHeader = new PacketHeader();
		packetHeader.m_PakcetType = packetType;

		// SizeOf ����Ͽ� �Ҵ��� �������� �ʴ� �޸��� ���� ����
		int headerSize = Marshal.SizeOf(packetHeader);
		int BodySize = Marshal.SizeOf(_packet);

		packetHeader.m_iSize = headerSize + BodySize;

		byte[] arr = new byte[packetHeader.m_iSize];

		// ���μ����� �������� �ʴ� �޸𸮿��� �޸𸮸� �Ҵ��մϴ�.
		IntPtr headerPtr = Marshal.AllocHGlobal(headerSize);
		IntPtr bodyPtr = Marshal.AllocHGlobal(BodySize);

		// �����Ǵ� ��ü�� �����͸� �������� �ʴ� �޸� �������� �������մϴ�.

		Marshal.StructureToPtr(packetHeader, headerPtr, true);
		Marshal.StructureToPtr(_packet, bodyPtr, true);

		Marshal.Copy(headerPtr, arr, 0, headerSize);
		Marshal.Copy(bodyPtr, arr, headerSize, BodySize);

		// ���μ����� �Ҵ��� �������� �ʴ� �޸𸮸� ������ŵ�ϴ�.
		Marshal.FreeHGlobal(headerPtr);
		Marshal.FreeHGlobal(bodyPtr);

		SocketManager.Instance.Send(arr);
	}
}