// HKSock.cpp : ���� �����Դϴ�.
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
	((CHawkeye_clientApp*)AfxGetApp())->ReceiveData(); // �����͸� �д´�.
	CAsyncSocket::OnReceive(nErrorCode);
}
void CHKSock::OnClose(int nErrorCode)
{
	((CHawkeye_clientApp*)AfxGetApp())->DisConnectServ();
	CAsyncSocket::OnClose(nErrorCode);
}
// CHKSock ��� �Լ�
