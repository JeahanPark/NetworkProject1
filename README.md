# 1.프로젝트 개요 및 구현 동기
프로젝트 설명 : 멀티플레이어 탑뷰 슈팅 게임입니다.

구현 동기 :
1. 클라이언트, 서버, 데이터베이스를 직접 설계하고 구현하여 게임 개발 과정에서 데이터가 어떻게 흐르고 처리되는지 종합적으로 이해하고자 하였습니다.
2. 유니티를 이용한 클라이언트와 C++ 기반의 서버를 통해 실시간으로 캐릭터의 이동이 동기화되도록 구현하였습니다.
3. 유니티에서 HLSL을 이용하여 간단하게 쉐이더 구현해봤습니다.

# 2.게임 플레이 및 목적
목적: 플레이어는 서로 파이어볼을 발사하여 상대방을 제압하고 점수를 획득하는 것을 목표로 합니다.

게임 방법:
+  이동: 'W', 'A', 'S', 'D' 키를 사용하여 캐릭터를 이동시킵니다.
+  공격: 마우스 클릭 방향으로 파이어볼을 발사합니다.
+  전략 요소: 게임 필드에서 획득할 수 있는 반사 아이템을 활용하여 상대방의 공격을 반사시켜 대응할 수 있습니다.

추가 기능:
+  커뮤니티 지원: 플레이어 간의 소통을 위한 회원가입, 로그인, 채팅 시스템을 구현하였습니다

# 3.기능과 기술 스택
+ 클라이언트 : C#, 유니티
+ 서버 : C++
+ DB : MS-SQL

1. 로그인 로직

전체적인 로직 흐름입니다.
클라 -> 서버 -> DB -> 서버 -> 클라

```csharp
// 로비 관리코드 클라이언트 코드
public class LobbyController : MonoDestroySingleton<LobbyController>
{
   private LobbyUIWorker m_LobbyUI = null;
   ...
   public void SendLogIn(string _strID, string _strPassword)
   {
        if (string.IsNullOrEmpty(_strID) ||
            string.IsNullOrEmpty(_strPassword))
        {
            Debug.Log("비밀번호나 아이디가 입력이안되있음");
            return;
        }
        LogInPacket packet = new LogInPacket();
        packet.m_UserID = _strID;
        packet.m_Password = _strPassword;

        Packet.SendPacket<LogInPacket>(packet, ePacketType.CToS_Login);
    }
}
'''cpp
// 서버 패킷 핸들러 코드
PacketHandler.cpp

void PacketHandler::Login(s_ServerSession _session, BasePacket* _packetData)
{
	LoginRequestPacket* packetData = (LoginRequestPacket*)_packetData;

	DBObject* dbObject = DataBaseManager().GetInstance()->PopDBObject();

	WCHAR UserID[USER_ID_LENGTH];
	SQLLEN outUserID = 0;
	dbObject->BindCol(1, SQL_C_WCHAR, USER_ID_LENGTH, &UserID, &outUserID);

	WCHAR Password[PASSWORD_LENGTH] = L"\n";
	SQLLEN outPassword = 0;
	dbObject->BindCol(2, SQL_C_WCHAR, PASSWORD_LENGTH, &Password, &outPassword);

	int iScore;
	SQLLEN outScoreLen = 0;
	dbObject->BindCol(3, SQL_C_LONG, SQL_INTEGER, &iScore, &outScoreLen);

	int iUserIndex;
	SQLLEN outUserIndexLen = 0;
	dbObject->BindCol(4, SQL_C_LONG, SQL_INTEGER, &iUserIndex, &outUserIndexLen);

	SQLLEN temp1 = 0;
	dbObject->BindParam(packetData->m_UserID, &temp1);

	auto query = L"SELECT * FROM [GameServer].[dbo].[Players] WHERE UserID = (?)";

	if (!dbObject->Query(query))
	{
		//cout << "Query Fail" << endl;
	}

	ePacketResult packetResult = ePacketResult::Fail;

	if (dbObject->IsValidData())
	{
		int result = wcsncmp(Password,  packetData->m_Password, PASSWORD_LENGTH);
		if (0 == result)
		{
			// 비밀번호가 같다.
			_session->SetUserData(UserID, iScore, iUserIndex);
			packetResult = ePacketResult::Success;
		}
	}

	DataBaseManager().GetInstance()->PushDBObject(dbObject);

	SendBuffer* pSendBuffer = LoginResultPacketCreate(_session, packetResult);
	_session->RegisterSend(pSendBuffer);
}
   
   
3. 실시간 이동동기화 로직
 
4. 반사 로직

# 4.스크린샷 및 동영상
게임플레이 동영상 링크입니다.
https://youtu.be/GegMbj8o-Xk?si=_Co855wo4Su7P2sH
