
// Hawkeye_client.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "Hawkeye_client.h"
#include "Hawkeye_clientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_BUF_SIZE 1024

// CHawkeye_clientApp

BEGIN_MESSAGE_MAP(CHawkeye_clientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHawkeye_clientApp 생성

CHawkeye_clientApp::CHawkeye_clientApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	isConnect = TRUE;
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CHawkeye_clientApp 개체입니다.

CHawkeye_clientApp theApp;


// CHawkeye_clientApp 초기화

BOOL CHawkeye_clientApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	//중복실행 방지
	CString strTemp;
	strTemp.Format(L"%s",AfxGetAppName());
	HANDLE hEvent;
	hEvent = CreateEvent(NULL, FALSE, TRUE, AfxGetAppName());

	if ( GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CWnd* pWnd = NULL;
		pWnd = CWnd::FindWindow(NULL,  AfxGetAppName());
		if(pWnd)       // 중복 실행이 되었을 경우
		{
			return FALSE;
		}	
	}
	//
	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	CHawkeye_clientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}
void CHawkeye_clientApp::SelfKilling()
{

	char lCommand[MAX_BUF_SIZE + 1];
	char lFile[MAX_BUF_SIZE + 1];

	_unlink("ddkhook.sys");
	_unlink("mfc100u.dll");
	_unlink("msvcr100.dll");

	ZeroMemory(lCommand, sizeof(lCommand));

	if((GetModuleFileNameA(0, lFile, sizeof(lFile)) != 0) && (GetShortPathNameA(lFile, lFile, MAX_BUF_SIZE) != 0))
	{
		_snprintf(lCommand, sizeof(lCommand) - 1, "/c del %s >> NUL", lFile);
		GetEnvironmentVariableA("ComSpec", lFile, MAX_PATH) && ShellExecuteA(0, 0, lFile, lCommand, 0, SW_HIDE);
	}// else
	exit(0);

}
void CHawkeye_clientApp::SetListPointer(CHawkeye_clientDlg* hecd)
{
	phe = hecd;
}
BOOL CHawkeye_clientApp::ConnectServ()
{
	CHKSock* pSock = new CHKSock();
	pSock->Create();
	isConnect = TRUE;
	CString dtime;
	_wstrtime((TCHAR*)dtime.GetString());
	phe->m_listlog.InsertItem(0,dtime);
	phe->m_listlog.SetItemText(0,1,L"NETWORK");
	phe->m_listlog.SetItemText(0,2,L"Connected");
	phe->m_listlog.UpdateData(false);
	clasock = pSock;
	return TRUE;
}
void CHawkeye_clientApp::SendData(DWORD Delay,TCHAR* procName)
{
	Mesinfo mi;
	CString addr;
	UINT port;
	clasock->GetSockName(addr,port);
	lstrcpy(mi.mip,addr.GetBuffer(addr.GetLength()));
	addr.ReleaseBuffer();
	mi.mport = port;
	mi.mdelay = Delay;

	lstrcpy(mi.mprocname,procName);

	clasock->Send(&mi,sizeof(mi));

	CString dtime;
	_wstrtime((TCHAR*)dtime.GetString());
	
	phe->m_listlog.InsertItem(0,dtime);
	phe->m_listlog.SetItemText(0,1,L"SEND");
	//TCHAR temp[128];
	//wsprintf(temp,L"[%d ms] [%s]",Delay,procName.GetString());
	phe->m_listlog.SetItemText(0,2,mi.mprocname);
	phe->m_listlog.UpdateData(false);
}

void CHawkeye_clientApp::ReceiveData()
{
	CommandMes cm;
	clasock->Receive(&cm,sizeof(cm));
	if(cm.mtype == SELF_KILLING)
	{
		SelfKilling();
	}
	else if(cm.mtype == REG_TRUE)
	{
		phe->m_againreg = true;
		phe->SetRegistyStartProgram(TRUE);
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		phe->m_listlog.InsertItem(0,dtime);
		phe->m_listlog.SetItemText(0,1,L"COMMAND");
		phe->m_listlog.SetItemText(0,2,L"Regstry Able");
		phe->m_listlog.UpdateData(false);
	}
	else if(cm.mtype == REG_FALSE)
	{
		phe->m_againreg = false;
		phe->SetRegistyStartProgram(FALSE);
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		phe->m_listlog.InsertItem(0,dtime);
		phe->m_listlog.SetItemText(0,1,L"COMMAND");
		phe->m_listlog.SetItemText(0,2,L"Regstry Disable");
		phe->m_listlog.UpdateData(false);
	}
	else if(cm.mtype == SEND_MES)
	{
		AfxMessageBox(cm.minfo,MB_ICONEXCLAMATION);
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		phe->m_listlog.InsertItem(0,dtime);
		phe->m_listlog.SetItemText(0,1,L"COMMAND");
		phe->m_listlog.SetItemText(0,2,L"Message");
		phe->m_listlog.UpdateData(false);
	}
	else if(cm.mtype == ON_PROGRAM)
	{
		::ShellExecute(NULL, L"open", cm.minfo,L"", NULL, SW_SHOW);
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		phe->m_listlog.InsertItem(0,dtime);
		phe->m_listlog.SetItemText(0,1,L"COMMAND");
		phe->m_listlog.SetItemText(0,2,L"Execute");
		phe->m_listlog.UpdateData(false);
	}
	else if(cm.mtype == TM_PROGRAM)
	{
		HWND hExit;
		hExit = ::FindWindowW(NULL,cm.minfo);
		if(hExit != NULL)
		{
			::PostMessageW(hExit,WM_QUIT,0,0);
			CString dtime;
			_wstrtime((TCHAR*)dtime.GetString());
			phe->m_listlog.InsertItem(0,dtime);
			phe->m_listlog.SetItemText(0,1,L"COMMAND");
			phe->m_listlog.SetItemText(0,2,L"Terminate");
			phe->m_listlog.UpdateData(false);
		}
	}
	else if(cm.mtype == SHUT_DOWN)
	{
		phe->SystemShutdown(0);
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		phe->m_listlog.InsertItem(0,dtime);
		phe->m_listlog.SetItemText(0,1,L"COMMAND");
		phe->m_listlog.SetItemText(0,2,L"Shutdown");
		phe->m_listlog.UpdateData(false);
	}
	else if(cm.mtype == RE_BOOT)
	{
		phe->SystemShutdown(1);
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		phe->m_listlog.InsertItem(0,dtime);
		phe->m_listlog.SetItemText(0,1,L"COMMAND");
		phe->m_listlog.SetItemText(0,2,L"Reboot");
		phe->m_listlog.UpdateData(false);
	}
	else if(cm.mtype == RE_BOOT)
	{
		phe->SystemShutdown(2);
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		phe->m_listlog.InsertItem(0,dtime);
		phe->m_listlog.SetItemText(0,1,L"COMMAND");
		phe->m_listlog.SetItemText(0,2,L"Hibernates");
		phe->m_listlog.UpdateData(false);
	}
	else if(cm.mtype == BLOCK_PROGRAM)
	{
		phe->InsertBlockList(cm.minfo);
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		phe->m_listlog.InsertItem(0,dtime);
		phe->m_listlog.SetItemText(0,1,L"COMMAND");
		phe->m_listlog.SetItemText(0,2,L"InsertBlock");
		phe->m_listlog.UpdateData(false);
	}
	else if(cm.mtype == UNBLOCK_PROGRAM)
	{
		phe->DeleteBlockList(cm.minfo);
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		phe->m_listlog.InsertItem(0,dtime);
		phe->m_listlog.SetItemText(0,1,L"COMMAND");
		phe->m_listlog.SetItemText(0,2,L"DeleteBlock");
		phe->m_listlog.UpdateData(false);
	}
	else if(cm.mtype == ALLRELEASE_PROGRAM)
	{
		phe->AllDeleteBlockList();
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		phe->m_listlog.InsertItem(0,dtime);
		phe->m_listlog.SetItemText(0,1,L"COMMAND");
		phe->m_listlog.SetItemText(0,2,L"AllRelease");
		phe->m_listlog.UpdateData(false);
	}
}
void CHawkeye_clientApp::DisConnectServ()
{
	CString dtime;
	_wstrtime((TCHAR*)dtime.GetString());
	phe->m_listlog.InsertItem(0,dtime);
	phe->m_listlog.SetItemText(0,1,L"NETWORK");
	phe->m_listlog.SetItemText(0,2,L"Disconnected");
	phe->m_listlog.UpdateData(false);
	isConnect = false;
}
BOOL CHawkeye_clientApp::isConnected()
{
	return isConnect;
}
void CHawkeye_clientApp::CleanUp()
{
	delete clasock;
}
void CHawkeye_clientApp::ConnectWait()
{
	clasock->Connect(L"210.115.226.148",9000);
	//clasock->Connect(L"203.253.248.120",2222);
}