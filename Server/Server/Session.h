#pragma once


const int BUFSIZE = 1000;

class Session
{
public:
    SOCKET GetSocket() { return m_Socket; }

public:
    char m_recvBuffer[BUFSIZE];

private:
    
    SOCKET m_Socket = INVALID_SOCKET;
    /*
    SOCKET socket = INVALID_SOCKET;
    char recvBuffer[BUFSIZE] = {};
    int recvBytes = 0;*/
};

