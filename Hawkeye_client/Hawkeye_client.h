
// Hawkeye_client.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.
#include "HKSock.h"
#include "Hawkeye_clientDlg.h"
// CHawkeye_clientApp:
// �� Ŭ������ ������ ���ؼ��� Hawkeye_client.cpp�� �����Ͻʽÿ�.
//
enum ECMD
{
	SELF_KILLING,
	REG_TRUE,
	REG_FALSE,
	SEND_MES,
	SEND_FILE,
	ON_PROGRAM,
	TM_PROGRAM,
	BLOCK_PROGRAM,
	UNBLOCK_PROGRAM,
	ALLRELEASE_PROGRAM,
	SHUT_DOWN,
	RE_BOOT,
	HIBER_MODE,
	UP_DATE
};
struct CommandMes
{
	UINT mtype;
	TCHAR minfo[512];
};
class CHawkeye_clientApp : public CWinApp
{
public:
	CHawkeye_clientApp();
public:
	//�ڻ��ڵ�
	void SelfKilling();
public:
	//��Ʈ��ũ�ڵ�
	void SetListPointer(CHawkeye_clientDlg* hecd);
	BOOL ConnectServ();
	void SendData(DWORD Delay,TCHAR* procName);
	void ReceiveData();
	void DisConnectServ();
	BOOL isConnected();
	void CleanUp();
	void ConnectWait();
private:
	CHawkeye_clientDlg* phe;
	CHKSock* clasock;
	BOOL isConnect;
	// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CHawkeye_clientApp theApp;