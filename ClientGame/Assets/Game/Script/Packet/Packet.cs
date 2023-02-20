using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System;
using UnityEngine;

public enum ePacketType
{
	NONE = 0,

	// 결과 보낼때 쓰자
	Result,

	// 데이터는 안보내고 간단하게 신호만 보낼경우 이걸로 쓰자
	Signal,


	// 서버에서 클라로

	SToC_Chatting,
	SToC_PacketResult,
	SToC_LoginResult,
	SToC_InGameUpdate,
	SToC_InitialInGameData,
	SToC_NewUserInteraction,
	SToC_RecivedDamage,
	// 서버에서 클라로

	// 클라에서 서버로

	CToS_Login,
	CToS_Chatting,
	CToS_UserRegister,
	CToS_MyUserMove,
	// 클라에서 서버로

	END,
}

// 결과에 대한 enum
public enum ePacketResult
{
	NONE = 0,
	Success,
	Fail,

	// 채팅방 결과
	ChattingRoomEnter_Not_Login,
	ChattingRoomEnter_Already_In,
	ChattingRoomExit_Not_Exist,

	// 인게임 진입 나가기에 대한 결과
	InGameEnter_Not_Login,
	InGameEnter_Already_In,
	InGameEnter_InChattingRoom,
	InGameExit_Not_Exist,
};

// 데이터는 안보내고 간단하게 신호만 보낼경우 이걸로 쓰자
public enum ePacketSignal
{
	NONE = 0,

	// 채팅방 결과
	Signal_ChattingRoomEnter,
	Signal_ChattingRoomExit,
	Signal_ChattingRoomExit_Not_Exist,

	// 인게임 입장, 나가기 결과
	Signal_InGameEnter,
	Signal_InGameExit,

	Signal_InitialInGameData
};

public interface PacketListCount
{
	int GetListCount();
}

// 아... 나눌껄....
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct PacketHeader
{
	public ePacketType m_PakcetType;

	//	헤더 패킷 + 내용 패킷
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
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
public struct V2ChattingPacket
{
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
	public string chattingContent;
}


[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
public struct UserRegistPacket
{
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_UserID;
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_Password;

	public int Score;
};

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct InGameEnterPacket
{
	public int m_UserIndex;
};

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
public struct LogInPacket
{
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_UserID;
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_Password;
};
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
public struct LoginResultPacket
{
	public ePacketType m_eTargetPakcetType;
	public ePacketResult m_eResult;

	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_UserID;
	public int Score;
	public int m_iUserIndex;
};

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct PacketResult
{
	public ePacketType		m_eTargetPakcetType;
	public ePacketResult	m_eResult;

	public ePacketSignal	m_SignalType;
};

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct SignalPacket
{
	public ePacketSignal m_ePacketSignal;
};

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct InteractionData
{
	public int							m_iInteractionIndex;
	public eInteractionType				m_eType;
	private byte						m_ValidLife;
	public Vector3						m_vPos;
	public Vector3						m_vDir;
	public float						m_fMoveSpeed;

	public bool	VaildLife
    {
		get
        {
			return m_ValidLife == 0 ? false : true;

		}
    }
};

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct InGameUpdatePacket : PacketListCount
{
	public int m_iInteractionCount;

    public int GetListCount()
    {
        return m_iInteractionCount;
    }
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct InGameEnterSuccess
{
	public int m_iInteractionIndex;
	public eInteractionType m_eType;
};
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct MyUserMovePacket
{
	public Vector3 m_vDir;
	public float m_fMoveSpeed;
};
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
public struct InitialInGameData
{
	public InteractionData m_UserData;

	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
	public string m_strNickName;
};
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct InitialInGameDataPacket : PacketListCount
{
	public int m_iMyInteractionIndex;
	public int m_iUserCount;

	public int GetListCount()
	{
		return m_iUserCount;
	}
};
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct NewUserPacket
{
	public InitialInGameData m_InitData;
};

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct RecivedDamagePacket
{
	public int m_iInteractionIndex;
	public float m_fReciveDamage;
};
public class Packet
{
	public static T BufferToPacket<T>(byte[] _buffer, int _iHeaderSize) where T : struct
    { 
		int size = Marshal.SizeOf(typeof(T));
		IntPtr ptr = Marshal.AllocHGlobal(size);
		Marshal.Copy(_buffer, _iHeaderSize, ptr, size);
		T packet = (T)Marshal.PtrToStructure(ptr, typeof(T));
		Marshal.FreeHGlobal(ptr);

		return packet;
	}
    public static (T, U[]) BufferToPacket<T, U>(byte[] _buffer, int _iHeaderSize) where T : struct where U : struct
    {
        int sizeT = Marshal.SizeOf(typeof(T));
        int sizeU = Marshal.SizeOf(typeof(U));

		// 패킷 만들기
        IntPtr ptrT = Marshal.AllocHGlobal(sizeT);
        Marshal.Copy(_buffer, _iHeaderSize, ptrT, sizeT);
		T packetT = (T)Marshal.PtrToStructure(ptrT, typeof(T));
		Marshal.FreeHGlobal(ptrT);


		// 패킷의 리스트 만들기
		PacketListCount count = packetT as PacketListCount;
		int iTotalCountU = count.GetListCount();

		int iListStartIndex = sizeT + _iHeaderSize;

		U[] list = new U[iTotalCountU];
		IntPtr ptrU = Marshal.AllocHGlobal(sizeU * iTotalCountU);

		for ( int i = 0; i < iTotalCountU; ++i)
        {
			int size = iListStartIndex + sizeU * i;

			Marshal.Copy(_buffer, size, ptrU, sizeU);
			list[i] = (U)Marshal.PtrToStructure(ptrU, typeof(U));
		}
		Marshal.FreeHGlobal(ptrU);


		return (packetT, list);
    }
    public static bool ReceviePacketHandle(ReceiveBuffer _buffer)
    {
		int iHeaderSize = 0;
		// 패킷 헤더 사이즈 체크
		{
			iHeaderSize = Marshal.SizeOf(typeof(PacketHeader));
			if (_buffer.m_iReadPos < iHeaderSize)
				return false;
		}

		// 패킷 사이즈가 부족하다.
		PacketHeader packetHeader;
		{
			packetHeader = BufferToPacket<PacketHeader>(_buffer.m_recvBuffer, 0);
			if (_buffer.m_iReadPos < packetHeader.m_iSize)
            {
				Debug.Log("Fail packetType = " + packetHeader.m_PakcetType.ToString() + ", Size = " + packetHeader.m_iSize);
				return false;
			}
				
		}
		Debug.Log("Success packetType = " + packetHeader.m_PakcetType.ToString() + ", Size = " + packetHeader.m_iSize);
		byte[] buffer = _buffer.PopPacketByte(packetHeader.m_iSize);

		// 패킷 
		PacketHandler.PacketHandling(iHeaderSize, buffer, packetHeader);
		
		return true;
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