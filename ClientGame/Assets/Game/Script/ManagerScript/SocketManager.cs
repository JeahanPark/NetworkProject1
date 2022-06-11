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

    private const string m_strServerAddress = "127.0.0.1";
    private const int m_iPort = 7777;

    private string m_strMessage = string.Empty;
    private int m_ReceiveCount = 0;

    public bool IsConnet()
    {
        return m_socket.Connected;
    }

    public override void Destroy()
    {
        if(m_socket != null)
        {
            m_socket.Close();
        }
    }

    public override void Init()
    {
    }

    public void Send(string _strMessage)
    {
        if(!string.IsNullOrEmpty(m_strMessage))
            return;

        m_strMessage = _strMessage;

        StartCoroutine(CoSendData());
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
        while(true)
        {
            byte[] bytes = new byte[1000];

            IAsyncResult async = null;
            try
            {
                async = m_socket.BeginReceive(bytes, 0, bytes.Length, 0, null, null);
            }
            catch (SocketException e)
            {
                Debug.Log(string.Format("{0} Error Code: {1}.", e.Message, e.ErrorCode));

                m_socket.EndReceive(async);
                yield break;
            }

            // 뭔가 받은게 있다.
            while (!async.IsCompleted)
                yield return null;

            m_socket.EndReceive(async);

            PacketReceiveHandler(bytes);
        }
    }
    public IEnumerator CoSendData()
    {
        if(!string.IsNullOrEmpty(m_strMessage))
        {
            IAsyncResult asyncResult = null; 
            byte[] msg = Encoding.UTF8.GetBytes(m_strMessage);
            try
            {
                asyncResult = m_socket.BeginSend(msg, 0, msg.Length, 0, null, null);
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
            Debug.Log("Size Leng :" + size);
            m_strMessage = string.Empty;
        }
    }

    private void PacketReceiveHandler(byte[] buffer)
    {
        string strReceiveDaata = Encoding.UTF8.GetString(buffer);

        // 일단 대화밖에 없어서 이렇게 처리
        // 근데 어케해야될지 모르겠어...
        LobbyController.Instance.ReceiveChattingMessage(strReceiveDaata);
    }
}
