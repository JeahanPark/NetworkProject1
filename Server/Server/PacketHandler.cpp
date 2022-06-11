#include "pch.h"
#include "PacketHandler.h"

PacketHandler::PacketHandler()
{
	//    // CreateIoCompletionPort함수는 2가지 용도로 사용한다.
	// 1. IOCP커널 오브젝트를 생성하기위해 사용된다.
	//    처음에 한번 호출을 해줘야한다.
	//    호출후에는 IOCP 핸들을 반환한다.
	// 2. IOCP 완료 포트 핸들과 소켓 핸들을 연결하면 프로세스에서 
	//    해당 소켓 핸들과 관련된 비동기 I/O 작업의 완료 알림을 받을수 있다.

}

