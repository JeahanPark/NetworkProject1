using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net.Sockets;
using System.Net;
using System;
using System.Threading;
using System.Text;



public class SocketManager : MonoSingleton<SocketManager>
{
    private Socket m_socket = null;

    private const string m_strServerAddress = "3.39.194.36";
    private const int m_iPort = 3389;

    private Queue<byte[]> m_queBuffer = new Queue<byte[]>(); 
    private string m_strMessage = string.Empty;

    private ReceiveBuffer m_receiveBuffer = new ReceiveBuffer();

    public bool IsConnet()
    {
        return m_socket != null && m_socket.Connected;
    }

    public override void Destroy()
    {
        if(IsConnet())
        {
            Disconnect();
        }
    }

    public override void Init()
    {
    }

    public void Send(byte[] _buffer)
    {
        if (!IsConnet())
            return;

        if (_buffer == null || _buffer.Length == 0)
            return;

        m_queBuffer.Enqueue(_buffer);

        StartCoroutine(CoSendData());
    }

    public void Disconnect()
    {
        if (!IsConnet())
            return;
        //Shutdown
        //
        // Socket에서 보내기 및 받기를 사용할 수 없도록 설정합니다.
        // 소켓을 닫기 전에 모든 데이터를 보내고 받도록 하려면 메서드를 호출 Disconnect 하기 전에 호출 Shutdown 해야 합니다.
        m_socket.Shutdown(SocketShutdown.Both);

        m_socket.Disconnect(false);

        Debug.Log("Disconnect!!!");
    }

    public void ServerConnect(System.Action<bool> _funResult)
    {
        if (m_socket != null)
        {
            Debug.Log("이미 연결되있다.");
            _funResult(false);
            return;
        }

        StartCoroutine(CoServerConnect(_funResult));
    }

    public IEnumerator CoServerConnect(System.Action<bool> _funResult)
    {
        IAsyncResult async = null;
        try
        {
            m_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            // IP(인터넷 프로토콜) 주소를 제공합니다.
            IPAddress ipAddress = IPAddress.Parse(m_strServerAddress);

            // 네트워크 엔드포인트를 IP 주소와 포트 번호로 나타냅니다.

            // 엔드포인트란?
            //이 클래스는 EndPoint 네트워크 리소스 또는 서비스를 나타내는 기본 클래스를 제공합니다
            //abstract . 하위 클래스는 네트워크 연결 정보를 결합하여 서비스에 대한 연결 지점을 형성합니다.

            IPEndPoint ipEndPoint = new IPEndPoint(ipAddress, m_iPort);

            m_socket.ReceiveTimeout = 1;

            async = m_socket.BeginConnect(ipEndPoint, null, null);
        }
        catch (Exception e)
        {
            Debug.Log("socket Connnet Fail" + e);
            _funResult(false);
            m_socket.EndConnect(async);
            m_socket = null;
            yield break;
        }

        while (!async.IsCompleted)
        {
            yield return null;
        }

        if(!m_socket.Connected)
        {
            _funResult(false);
            m_socket = null;
            yield break;
        }
        Debug.Log("Connecting to Server");
        m_socket.EndConnect(async);
        _funResult(true);

        StartCoroutine(CoReceiveData());
    }

    public IEnumerator CoReceiveData()
    {
        //while(true)
        //{
        //    byte[] bytes = new byte[EnumType.BUFF_MAX_SIZE];

        //    int iTotalRecevieBtye = 0;

        //    while(true)
        //    {
        //        IAsyncResult async = null;
        //        try
        //        {
        //            async = m_socket.BeginReceive(bytes, iTotalRecevieBtye, bytes.Length, 0, null, null);
        //        }
        //        catch (ArgumentOutOfRangeException e)
        //        {
        //            Debug.Log(string.Format("{0}, {1}", e.Message, iTotalRecevieBtye));

        //            int a = m_socket.EndReceive(async);
        //            yield break;
        //        }

        //        // 뭔가 받은게 있다.
        //        while (!async.IsCompleted)
        //            yield return null;

        //        // 연결이 끊겨있다.
        //        if (!IsConnet())
        //            break;

        //        int iRecevieBtye = m_socket.EndReceive(async);

        //        if (iRecevieBtye == 0)
        //            Disconnect();

        //        iTotalRecevieBtye += iRecevieBtye;


        //        // 패킷 사이즈 체크후
        //        // 패킷 데이터 처리
        //        if (Packet.ReceviePacketHandle(bytes, iTotalRecevieBtye))
        //        {
        //            // 패킷 보내는거 성공
        //            break;
        //        }
        //        else
        //        {
        //            // 패킷 사이즈 체크에서 실패
        //            continue;
        //        }
        //    }
        //}

        while(true)
        {
            IAsyncResult async = null;

            try
            {
                async = m_socket.BeginReceive(m_receiveBuffer.m_recvBuffer, m_receiveBuffer.m_iReadPos, EnumType.BUFF_MAX_SIZE, 0, null, null);
            }
            catch(ArgumentException e)
            {
                Debug.Log(e.Message + ", bufferSize = " + m_receiveBuffer.m_iReadPos);
                m_socket.EndReceive(async);
                Disconnect();
                yield break;
            }
            

            // 뭔가 받은게 있다.
            while (!async.IsCompleted)
                yield return null;

            // 연결이 끊겨있다.
            if (!IsConnet())
                break;

            int iRecevieBtye = m_socket.EndReceive(async);

            if (iRecevieBtye == 0)
                Disconnect();

            m_receiveBuffer.m_iReadPos += iRecevieBtye;

            while(true)
            {
                // 패킷 사이즈 체크후
                // 패킷 데이터 처리
                if (!Packet.ReceviePacketHandle(m_receiveBuffer))
                {
                    // 패킷 사이즈 체크에서 실패
                    // 더이상 보낼게 없으니 다시 받자.
                    break;
                }

                // 성공했다 또 받을게 있을수도 있으니까 또 확인해보자
            }
        }
    }
    public IEnumerator CoSendData()
    {
        byte[] buffer = m_queBuffer.Dequeue();

        if (buffer == null)
            yield break;

        IAsyncResult asyncResult = null; 
        try
        {
            asyncResult = m_socket.BeginSend(buffer, 0, buffer.Length, 0, null, null);
        }
        catch (SocketException e)
        {
            Debug.Log(string.Format("{0} Error Code: {1}.", e.Message, e.ErrorCode));
            m_socket.EndSend(asyncResult);
            m_strMessage = string.Empty;
            yield break;
        }

        while(!asyncResult.IsCompleted)
        {
            yield return null;
        }

        int size = m_socket.EndSend(asyncResult);
        //Debug.Log("Size Leng :" + size);
        m_strMessage = string.Empty;
        
    }
}
