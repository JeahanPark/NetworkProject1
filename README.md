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
<details>
<summary>1.로그인 로직</summary>
	
**로그인 프로세스 상세 설명**<br>
1.인증 요청: 클라이언트는 로그인을 시도하며, 사용자 ID(닉네임), 비밀번호 및 패킷 타입을 서버로 전송합니다.<br>
2.데이터 검색: 서버는 데이터베이스의 Players 테이블에서 해당 사용자 ID로 사용자 데이터를 조회합니다.<br>
3.비밀번호 검증: 서버는 데이터베이스에서 검색된 비밀번호와 클라이언트가 전송한 비밀번호를 비교합니다.<br>
4.응답 전송: 검증 결과에 따라 서버는 클라이언트에 로그인 성공 또는 실패 여부를 알리는 응답을 전송합니다.<br>

연관 클래스
https://github.com/ClientGame/Assets/Game/Script/Controller/LobbyController.cs
```csharp
// 클라이언트 로비 관리코드
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

	// 서버에 userID(닉네임), 패스워드, 패킷타입을 전달합니다.
        Packet.SendPacket<LogInPacket>(packet, ePacketType.CToS_Login);
    }
}
``````

```cpp
// 서버 패킷 핸들러 코드
PacketHandler.cpp

void PacketHandler::Login(s_ServerSession _session, BasePacket* _packetData)
{
	// 클라이언트에서 넘겨준 패킷을 닉네임과 패스워드를 담은 구조체로 형변환하여 데이터를 가져옵니다.
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

	// DB의 Players테이블에서 UserID로 찾아 데이터를 가져옵니다.
	auto query = L"SELECT * FROM [GameServer].[dbo].[Players] WHERE UserID = (?)";

	if (!dbObject->Query(query))
	{
		//cout << "Query Fail" << endl;
	}

	ePacketResult packetResult = ePacketResult::Fail;

	// 비밀번호 체크를 합니다.
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

	// 성공적으로 로그인을 하면 ePacketResult::Success을 클라이언트에 넘겨주고
	// 클라이언트는 패킷 성공여부에 따라 동작을 합니다.
	SendBuffer* pSendBuffer = LoginResultPacketCreate(_session, packetResult);
	_session->RegisterSend(pSendBuffer);
}
``````
</details>

<details>
<summary>2. 실시간 이동동기화 로직</summary>

</details>

<details>
<summary>3. 반사 로직</summary>

</details>


# 4.스크린샷 및 동영상
게임플레이 동영상 링크입니다.
https://youtu.be/GegMbj8o-Xk?si=_Co855wo4Su7P2sH
