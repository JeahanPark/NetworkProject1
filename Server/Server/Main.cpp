#include "pch.h"

void Stop()
{
    int a = 0;
    cin >> a;
}

int main()
{
    cout << "Server Start!!!" << endl;
    /*
    1. 윈도우 소켓 생성
    2. 소켓 생성(서버)
    3. 소켓 주소 생성(서버)
    4. Bind함수 호출
    5. Listen함수 호출
    6. CreateIoCompletionPort함수 호출

    // 클라가 접속했을경우
    7. Accept함수 호출
    8. CreateIoCompletionPort함수 호출
    9. WSARecv함수 호출
    */

    // Select
    // WSAEvent

    //DB세팅
    //if (!DataBaseManager().GetInstance()->Connect())
    //{
    //    cout << "DB Connect Fail" << endl;
    //    return 0;
    //}

    //DBObject* object = DataBaseManager().GetInstance()->PopDBObject();

    //int32 outGold = 0;
    //SQLLEN outGoldLen = 0;
    //ASSERT_CRASH(dbConn->BindCol(2, SQL_C_LONG, sizeof(outGold), &outGold, &outGoldLen));

    //DataBaseManager().GetInstance()->BindCol(1 ,SQL_VARCHAR,)

    //DataBaseManager().GetInstance()->UseDB();

    //WCHAR UserID[50];
    //SQLLEN outUserID = 0;
    //object->BindCol(1, SQL_C_WCHAR, 50, &UserID, &outUserID);

    //WCHAR Password[50];
    //SQLLEN outPassword = 0;
    //object->BindCol(2, SQL_C_WCHAR, 50, &Password, &outPassword);

    //int Score;
    //SQLLEN outScoreLen = 0;
    //object->BindCol(3, SQL_C_LONG, SQL_INTEGER, &Score, &outScoreLen);

    //object->Query(L"SELECT * FROM [GameServer].[dbo].[Players]");

    //wcout.imbue(locale("kor"));

    //while (object->Fetch())
    //{
    //    wcout << "UserID  : " << UserID << ", Password  : "
    //        << Password << ", Score  : " << Score << endl;
    //}

    //return 0;
    //wcout.imbue(locale("kor"));
    // 윈도우 소켓 생성
    {
        SocketUtil::InitWinSock();
    }
    
    //소켓 클래스는 네트워크 통신을 하기 위해 필요한 함수나 변수를 제공해주는 클래스입니다.
    SOCKET listenSocket = SocketUtil::SocketCreate();

    SOCKADDR_IN serverAddr;
    {
        // SOCKADDR_IN
        // 소켓의 주소를 표현하는 구조체다.
        // 범용 주소 구조체인 SOCKADDR를 사용하지만 주소체계에 따라 전용 구조체를 만든다
        // SOCKADDR_IN은 IPv4 주소체계 전용으로 사용하기 위한 구조체다.
        
        memset(&serverAddr, 0, sizeof(serverAddr));

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
        serverAddr.sin_port = ::htons(7777);
    }


    int iResult = 0;
    {
        

        iResult = ::bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
        // bind 함수 는 소켓 주소 표현 구조체와 소켓을 연결하기위해 사용하는것
        if (iResult == SOCKET_ERROR)
        {
            cout << "bind Fail" << endl;
            cout << WSAGetLastError() << endl;
            
            closesocket(listenSocket);
            WSACleanup();
            Stop();
            
            return 0;
        }

        cout << "bind result : " << iResult << endl;
    }



    {
        // 다른 프로세스?(ex.클라이언트)에서 연결요청을 했을경우 처리가 가능한 상태
        //
        iResult = ::listen(listenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR)
        {
            cout << "listen Fail" << endl;
            cout << WSAGetLastError() << endl;

            closesocket(listenSocket);
            WSACleanup();
            Stop();

            return 0;
        }

        cout << "listen result : " << iResult << endl;
    }

    
    // CreateIoCompletionPort함수는 2가지 용도로 사용한다.
    // 1. IOCP커널 오브젝트를 생성하기위해 사용된다.
    //    처음에 한번 호출을 해줘야한다.
    //    호출후에는 IOCP 핸들을 반환한다.
    // 2. IOCP 완료 포트 핸들과 소켓 핸들을 연결하면 프로세스에서 
    //    해당 소켓 핸들과 관련된 비동기 I/O 작업의 완료 알림을 받을수 있다.

    HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    int count = thread::hardware_concurrency();
    cout << "thread max count : " << count << endl;

    // IOCP를 받을 쓰레드를 돌린다.
    for (int i = 0; i < 5; ++i)
    {
        g_ThreadManager->Run([&iocpHandle]
        {
            SocketUtil::SocketEventHandle(iocpHandle);
        });
    }
    
    // 업데이트 실행
    g_ThreadManager->Run([&iocpHandle]
    {
        InGameUpdateManager::GetInstance()->InGameUpdate();
    });

    // 컨텐츠 매니저 세팅
    ContentsManager::GetInstance()->InitContents();

    cout << "game start" << endl;

    // 여기서 클라이언트에서 연결을 받는다.
    while (true)
    {
        // 클라이언트 소켓 주소
        SOCKADDR_IN clinetAddr;
        int addrLen = sizeof(clinetAddr);

        // 외부에서 연결시도를 했을경우 시도한 해당 소켓을 뱉어준다.
        SOCKET clientSocket = ::accept(listenSocket, (sockaddr*)&clinetAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET)
            return 0;

        shared_session session = ServerSessionManager().GetInstance()->CreateSession();
        session->InitSession(iocpHandle, clientSocket);

        //cout << session->GetSessionNumber() << " Join!!!" << endl;

        //// 2번의 형식
        //// IOCP 완료 포트 핸들과 소켓 핸들을 연결하면 프로세스에서 해당 소켓 핸들과 관련된 비동기 I/O 작업의 완료 알림을 받을수 있다.
        //// 핸들(소켓)을 iocp와 연결
        //Crea.teIoCompletionPort((HANDLE)clientSocket, iocpHandle, (ULONG_PTR)session, 0);

        //{
        //    // 버퍼의 주소와 버퍼의 길이를 저장
        //    // 주소와 길이를 저장하는이유
        //    // WSABUF 배열 형식으로 한번에 전달 가능하게끔 하기위해
        //    // 버퍼를 모아서 한번에 출력하는기법이 Scatter-Gather

        //    session->RegisterReceive();
        //}
    }
    
    // 윈도우 소켓 해제
    WSACleanup();
}