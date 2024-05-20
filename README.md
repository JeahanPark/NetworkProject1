# 1.프로젝트 개요 및 구현 동기
프로젝트 설명 : 멀티플레이어 탑뷰 슈팅 게임입니다.

구현 동기 :
1. 클라이언트, 서버, 데이터베이스를 직접 설계하고 구현하여 게임 개발 과정에서 데이터가 어떻게 흐르고 처리되는지 종합적으로 이해하고자 하였습니다.
2. 유니티를 이용한 클라이언트와 C++ 기반의 서버를 통해 실시간으로 캐릭터의 이동이 동기화되도록 구현하였습니다.
3. 유니티에서 HLSL을 이용하여 간단하게 쉐이더 구현해봤습니다.

# 2.게임 플레이 및 목적
목적: 플레이어는 서로 파이어볼을 발사하여 상대방을 제압하고 점수를 획득하는 것을 목표로 합니다.

**게임플레이 동영상 링크입니다.** <br>
https://youtu.be/GegMbj8o-Xk?si=_Co855wo4Su7P2sH

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
1. 인증 요청: 클라이언트는 로그인을 시도하며, 사용자 ID(닉네임), 비밀번호 및 패킷 타입을 서버로 전송합니다
2. 데이터 검색: 서버는 데이터베이스의 Players 테이블에서 해당 사용자 ID로 사용자 데이터를 조회합니다.
3. 비밀번호 검증: 서버는 데이터베이스에서 검색된 비밀번호와 클라이언트가 전송한 비밀번호를 비교합니다.
4. 응답 전송: 검증 결과에 따라 서버는 클라이언트에 로그인 성공 또는 실패 여부를 알리는 응답을 전송합니다.

[LobbyController.cs](https://github.com/JeahanPark/NetworkProject1/blob/master/ClientGame/Assets/Game/Script/Controller/LobbyController.cs)
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
[PacketHandler.cpp](https://github.com/JeahanPark/NetworkProject1/blob/master/Server/TestClient/ClientPacketHandler.cpp)
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

**이동 동기화 로직 상세 설명**<br>
1. 사용자 입력 전송: 사용자가 방향키를 누르면 이동 방향, 속도, 바라보는 방향, 그리고 패킷 타입을 서버로 전송합니다.
2. 서버 처리: 서버는 받은 패킷을 해당 세션의 UserController에 저장하여 사용자의 상태를 업데이트합니다.
3. 위치 갱신: 서버는 초당 60프레임으로 각 프레임마다 UserController의 데이터를 Transform에 적용하여 사용자의 위치와 바라보는 방향을 업데이트합니다.
4. 데이터 브로드캐스팅: 10 프레임마다, 서버는 모든 사용자에게 위치, 이동속도, 이동방향, 바라보는 방향 등의 정보를 전송합니다.
5. 위치 예측: 다른 사용자의 위치 정보를 직접 적용하지 않고, 추측항법을 사용하여 다음 움직일 위치를 미리 계산했고 이동의 반응성을 높였습니다.

**추측항법**<br>

[InGameController.cs](https://github.com/JeahanPark/NetworkProject1/blob/master/ClientGame/Assets/Game/Script/Controller/InGameController.cs)
```csharp
// 클라이언트 인게임 관리코드
public class InGameController : MonoDestroySingleton<LobbyController>
{
    // 이전 업데이트 패킷을 받은 간격 시간 오차 
    private float _fUpdateLatency = 0;
    private void Update()
    {
        _fUpdateLatency += Time.deltaTime;
    }

    public void ReceiveInGameUpdate(InGameUpdatePacket _packet, InteractionData[] _interactionPacketDatas)
    {
    	// 위치, 방향을 갱신하는 패킷을 받을경우
        m_InteractionWorker.UpdateInteraction(_packet, _interactionPacketDatas, _fUpdateLatency * 0.5f);
        _fUpdateLatency = 0;
    }
}
``````

[UserObject.cs](https://github.com/JeahanPark/NetworkProject1/blob/master/ClientGame/Assets/Game/Script/InGame/Interaction/User/UserObject.cs)
```csharp
// 클라이언트 유저 오브젝트
public class UserObject : InteractionObject
{
    protected const float m_fDeadRackoningDeltaTime = 0.1f;

    // 정보를 갱신 받는다.
    public override void UpdateInteraction(InteractionData _InteractionData, float _fUpdateLatency)
    {
        m_vMoveDir = _InteractionData.m_vMoveDir;
        m_fMoveSpeed = _InteractionData.m_fMoveSpeed;

        m_bValidLife = _InteractionData.VaildLife;

        if (MyInteraction)
        {
            transform.position = _InteractionData.m_vPos;
        }
        else
        {
            // 다른 유저 일경우
            // 추측하는 위치 : 현재위치 + 이동방향 * 이전 패킷을 받은 간격 시간 오차 
            m_bDeadRackoningMove = true;
            m_vDeadRackoningPos = _InteractionData.m_vPos + m_vMoveDir * _fUpdateLatency;
            m_vDeadRackoningDir = m_vDeadRackoningPos - transform.position;
            m_vDeadRackoningDir.Normalize();
        }
        // 회전은 추측항법을 적용하지않는다. 보간만 적용
        m_vRotateY = _InteractionData.m_vRotateY;
    }

    // 이동한다.
    protected override void Update()
    {
        // 이동해야한다.
        {

            if (m_fMoveSpeed > 0)
            {
                // 데드레커닝 이동이 우선
                if (m_bDeadRackoningMove && !MyInteraction)
                {
                    // 데드레커닝으로 구한 위치로 0.1f안에 움직인다.
                    transform.position += m_vDeadRackoningDir * m_fDeadRackoningDeltaTime;

                    if ((transform.position - m_vDeadRackoningPos).magnitude < 1f)
                    {
                        m_bDeadRackoningMove = false;
                    }
                }
                else
                {
                    transform.position += m_vMoveDir * (m_fMoveSpeed * Time.deltaTime);
                    m_fMoveSpeed -= 1 * Time.deltaTime;
                }
            }
            else
                m_bDeadRackoningMove = false;
        }
	...
    }
}
``````

</details>

<details>
<summary>3. 반사 로직</summary>
	
1.test01이 pdwer3376에게 파이어볼을 날리는 상황<br>
![123](https://github.com/JeahanPark/NetworkProject1/assets/76486230/60ef5c2a-43f6-49e8-8b9d-fca0d2a43276)

2.pdwer3376이 반사스킬 사용(검은색 원)<br>
![image](https://github.com/JeahanPark/NetworkProject1/assets/76486230/35d26571-7207-4bd9-89c0-f6e93ae847c7)

3.맞은 반향으로 검은색이 진해지는 효과가 있다<br>
![image](https://github.com/JeahanPark/NetworkProject1/assets/76486230/4e2bd3f0-cda1-4a63-8fad-423f87c4538a)


[UserObject.cs](https://github.com/JeahanPark/NetworkProject1/blob/master/ClientGame/Assets/Game/Shader/ReflectionEffect.shader)
```cpp
Shader "Custom/ReflectionEffect"
{
    Properties
    {
	// 반사 이미지
        _BaseTexture("BaseTexture", 2D) = "white" {}

	// 어느방향으로 맞았는지 방향좌표를 넘겨준다.
        _ReciveDirUV("ReciveDirUV", Vector) = (1,0,1,1)
    }

    float4 frag(Vertex input) : SV_Target
    {
	//_ReciveDirUV 값을 사용하여 주변 픽셀과의 거리를 계산하고, 이를 바탕으로 픽셀의 색상과 투명도를 조절합니다. 
	//이는 리플렉션 효과의 중심에서 멀어질수록 투명도가 증가하도록 설정됩니다.

	 float4 mainColor = SAMPLE_TEXTURE2D(_BaseTexture, sampler_BaseTexture, input.uv.xy);

         // 계산하기 쉽게 좌표기준을 바꾸자
         //float2 x = input.uv.x * 2 - 1;
         //float2 y = input.uv.y * 2 - 1;

         if (_ReciveDirUV.z == 0)
         {
              mainColor.a *= 0.6;
              return mainColor;
         }
         float x = abs(_ReciveDirUV.x - input.uv.x);
         float y = abs(_ReciveDirUV.y - input.uv.y);

         float distance = sqrt(x * x + y * y);

         if (distance < 0.5)
         {
              float alphaRatio = distance / 0.5;

              mainColor.r *= 1 - 0.9 * (1 - alphaRatio);

              mainColor.a *= 0.6 + 1.5 * (1 - alphaRatio);
          }
          else
              mainColor.a *= 0.6;

          return mainColor;
     }
}
``````
</details>

