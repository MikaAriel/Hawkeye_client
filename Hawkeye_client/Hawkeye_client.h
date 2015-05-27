
// Hawkeye_client.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include "HKSock.h"
#include "Hawkeye_clientDlg.h"
// CHawkeye_clientApp:
// 이 클래스의 구현에 대해서는 Hawkeye_client.cpp을 참조하십시오.
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
	//자살코드
	void SelfKilling();
public:
	//네트워크코드
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
	// 재정의입니다.
public:
	virtual BOOL InitInstance();

	// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern CHawkeye_clientApp theApp;