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
    private const int BUFF_MAX_SIZE = 1024;

    private Socket m_socket = null;

    private const string m_strServerAddress = "127.0.0.1";
    private const int m_iPort = 7777;

    private Queue<byte[]> m_queBuffer = new Queue<byte[]>(); 
    private string m_strMessage = string.Empty;

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
        // Socket���� ������ �� �ޱ⸦ ����� �� ������ �����մϴ�.
        // ������ �ݱ� ���� ��� �����͸� ������ �޵��� �Ϸ��� �޼��带 ȣ�� Disconnect �ϱ� ���� ȣ�� Shutdown �ؾ� �մϴ�.
        m_socket.Shutdown(SocketShutdown.Both);

        m_socket.Disconnect(false);

        Debug.Log("Disconnect!!!");
    }

    public void ServerConnect(System.Action<bool> _funResult)
    {
        if (m_socket != null)
        {
            Debug.Log("�̹� ������ִ�.");
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

            // IP(���ͳ� ��������) �ּҸ� �����մϴ�.
            IPAddress ipAddress = IPAddress.Parse(m_strServerAddress);

            // ��Ʈ��ũ ��������Ʈ�� IP �ּҿ� ��Ʈ ��ȣ�� ��Ÿ���ϴ�.

            // ��������Ʈ��?
            //�� Ŭ������ EndPoint ��Ʈ��ũ ���ҽ� �Ǵ� ���񽺸� ��Ÿ���� �⺻ Ŭ������ �����մϴ�
            //abstract . ���� Ŭ������ ��Ʈ��ũ ���� ������ �����Ͽ� ���񽺿� ���� ���� ������ �����մϴ�.

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
            byte[] bytes = new byte[BUFF_MAX_SIZE];

            int iTotalRecevieBtye = 0;

            while(true)
            {
                IAsyncResult async = null;
                try
                {
                    async = m_socket.BeginReceive(bytes, iTotalRecevieBtye, bytes.Length, 0, null, null);
                }
                catch (SocketException e)
                {
                    Debug.Log(string.Format("{0} Error Code: {1}.", e.Message, e.ErrorCode));

                    m_socket.EndReceive(async);
                    yield break;
                }

                // ���� ������ �ִ�.
                while (!async.IsCompleted)
                    yield return null;

                // ������ �����ִ�.
                if (!IsConnet())
                    break;

                int iRecevieBtye = m_socket.EndReceive(async);

                if (iRecevieBtye == 0)
                    Disconnect();

                iTotalRecevieBtye += iRecevieBtye;


                // ��Ŷ ������ üũ��
                // ��Ŷ ������ ó��
                if (Packet.ReceviePacketHandle(bytes, iTotalRecevieBtye))
                {
                    // ��Ŷ �����°� ����
                    break;
                }
                else
                {
                    // ��Ŷ ������ üũ���� ����
                    continue;
                }
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
        Debug.Log("Size Leng :" + size);
        m_strMessage = string.Empty;
        
    }
}
