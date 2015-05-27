// HKSock.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Hawkeye_client.h"
#include "HKSock.h"


// CHKSock

CHKSock::CHKSock()
{
}

CHKSock::~CHKSock()
{
}
void CHKSock::OnReceive(int nErrorCode)
{
	((CHawkeye_clientApp*)AfxGetApp())->ReceiveData(); // 데이터를 읽는다.
	CAsyncSocket::OnReceive(nErrorCode);
}
void CHKSock::OnClose(int nErrorCode)
{
	((CHawkeye_clientApp*)AfxGetApp())->DisConnectServ();
	CAsyncSocket::OnClose(nErrorCode);
}
// CHKSock 멤버 함수
