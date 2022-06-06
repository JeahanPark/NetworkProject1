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

    private int m_SendCount = 0;
    private int m_ReceiveCount = 0;

    public override void Destroy()
    {
        if(m_socket != null)
        {
            m_socket.Close();
        }
    }

    public override void Init()
    {
        ServerConnect();
    }

    public void Send()
    {
        m_SendCount++;
    }
    private void ServerConnect()
    {
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

            Debug.Log("Connecting to Server");

            m_socket.ReceiveTimeout = 1;

            m_socket.Connect(ipEndPoint);
        }
        catch (Exception e)
        {
            Debug.Log("socket Connnet Fail" + e);
        }
    }

    private void Update()
    {
        if (m_socket == null)
        {
            return;
        }
            

        if (!m_socket.Connected)
        {
            return;
        }
            


        if(m_SendCount > 0)
        {
            byte[] msg = Encoding.UTF8.GetBytes("This is a test");
            try
            {
                int i = m_socket.Send(msg);
            }
            catch (SocketException e)
            {
                Debug.Log(string.Format("{0} Error Code: {1}.", e.Message, e.ErrorCode));
            }

            m_SendCount--;
        }

        //byte[] bytes = new byte[256];

        //int i = m_socket.Receive(bytes);
        
    } 
}
