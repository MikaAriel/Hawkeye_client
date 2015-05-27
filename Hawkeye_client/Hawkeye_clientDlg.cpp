
// Hawkeye_clientDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Hawkeye_client.h"
#include "Hawkeye_clientDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHawkeye_clientDlg 대화 상자




CHawkeye_clientDlg::CHawkeye_clientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHawkeye_clientDlg::IDD, pParent), m_bHide(false), m_first(true), m_againreg(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	lastTime = timeGetTime();
}
CHawkeye_clientDlg::~CHawkeye_clientDlg()
{
		drvmgr.RemoveDriver();
}

void CHawkeye_clientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGO, m_logo);
	DDX_Control(pDX, IDC_LIST1, m_listlog);
}

BEGIN_MESSAGE_MAP(CHawkeye_clientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CHawkeye_clientDlg::OnBnClickedOk)

	ON_MESSAGE(WM_TRAYICON, OnTrayIcon)
	ON_COMMAND(WM_APP_EXIT, OnAppExit)
	ON_COMMAND(WM_DIALOG_SHOW, OnDialogShow)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CHawkeye_clientDlg::OnLvnItemchangedList1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CHawkeye_clientDlg 메시지 처리기

BOOL CHawkeye_clientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	HBITMAP hbit;
	hbit = ::LoadBitmapW(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP1));
	m_logo.SetBitmap(hbit);

	//시작프로그램 레지스트리 등록
	//SetRegistyStartProgram(TRUE);
	//
	m_listlog.InsertColumn(0,L"[Time]",LVCFMT_CENTER,80);
	m_listlog.InsertColumn(1,L"[Type]",LVCFMT_CENTER,80);
	m_listlog.InsertColumn(2,L"[Log]",LVCFMT_CENTER,100);

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_myTray.m_bHide = m_bHide;
	m_myTray.AddTrayIcon(GetSafeHwnd());	//시작과 동시에 트레이 아이콘 표시

	CString delay;
	_wstrtime((TCHAR*)delay.GetString());
	m_listlog.InsertItem(0,delay);
	m_listlog.SetItemText(0,1,L"MFC Init");
	m_listlog.SetItemText(0,2,L"Success");
	UpdateData(false);
	
	drvmgr.SetListPointer(&m_listlog);
	drvmgr.InitDriver();
	drvmgr.GetMyCode();
	drvmgr.ActivateBlock();

	lstrcpy(tpProcess,L"Hawkeye_client");

	//네트워크 시작
	((CHawkeye_clientApp*)AfxGetApp())->SetListPointer(this);
	((CHawkeye_clientApp*)AfxGetApp())->ConnectServ();

	SetTimer(001,500,NULL);
	SetTimer(002,100,NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CHawkeye_clientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CHawkeye_clientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHawkeye_clientDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnDialogShow();
}

//트레이 아이콘을 클릭했을 때의 메시지 핸들러
long CHawkeye_clientDlg::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
	m_myTray.ProcTrayMsg(GetSafeHwnd(), wParam, lParam);
	return 0;
}

//트레이 아이콘 팝업메뉴의 종료 메뉴 메시지 핸들러
void CHawkeye_clientDlg::OnAppExit(void)
{
	m_againreg = false;
	m_myTray.DelTrayIcon(GetSafeHwnd());
	SetRegistyStartProgram(FALSE);
	drvmgr.RemoveDriver();
	CDialog::OnCancel();
}

//트레이 아이콘 보이기/숨기기 메뉴 메시지 핸들러
void CHawkeye_clientDlg::OnDialogShow(void)
{
	if(!m_bHide) ShowWindow(false);	//보이는 상태라면 숨기고
	else ShowWindow(true);			//숨겨진 상태라면 보이게
	m_bHide = !m_bHide;
	m_myTray.m_bHide = m_bHide;
}

void CHawkeye_clientDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
void CHawkeye_clientDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
		case 001:
			CheckTopWindowTimer();
			BlockProgram();
			break;
		case 002:
			if(m_first)
			{
				OnDialogShow();
				m_first = false;
			}
			if(!((CHawkeye_clientApp*)AfxGetApp())->isConnected())
			{
				((CHawkeye_clientApp*)AfxGetApp())->CleanUp();
				((CHawkeye_clientApp*)AfxGetApp())->ConnectServ();
			}
			((CHawkeye_clientApp*)AfxGetApp())->ConnectWait();
			if(m_againreg)
			{
				SetRegistyStartProgram(TRUE);
			}
			break;
	}
	CDialog::OnTimer(nIDEvent);

}
void CHawkeye_clientDlg::CheckTopWindowTimer()
{
	CWnd* tphWnd;
	TCHAR nowProcess[512];
	tphWnd = GetForegroundWindow();

	if(tphWnd->GetSafeHwnd() == this->GetSafeHwnd())
	{
		return;
	}
	::GetWindowTextW(tphWnd->GetSafeHwnd(),nowProcess,512);
	if(lstrcmp(nowProcess,L"") == 0)
	{
		lstrcpy(nowProcess,L"대기");
	}
	if(lstrcmp(tpProcess,nowProcess) != 0)
	{
		DWORD thisTime = timeGetTime();
		((CHawkeye_clientApp*)AfxGetApp())->SendData(thisTime - lastTime,tpProcess);
		lastTime = thisTime;
		lstrcpy(tpProcess,nowProcess);
	}
}

BOOL CHawkeye_clientDlg::SetRegistyStartProgram(BOOL bAutoExec)
{
	 HKEY hKey;
     LONG lRes;
	 TCHAR procName[128];
	 TCHAR path[MAX_PATH];

	 DWORD dwd = 0;
	 lstrcpy(procName,L"Hawkeye_client.exe");
	 ::GetModuleFileNameW(NULL,path,MAX_PATH);


	 //UAC 우회레지스트리
	 /*if(RegOpenKeyExW(
		 HKEY_LOCAL_MACHINE,
		 L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Policies\\System",
		 0,
		 KEY_ALL_ACCESS,
		 &hKey
		 ) != ERROR_SUCCESS)
		 return FALSE;

	 lRes = RegSetValueEx(hKey,
		 L"ConsentPromptBehaviorAdmin",
		 0,
		 REG_DWORD,
		 (LPBYTE)&dwd,
		 sizeof(DWORD));

	 RegCloseKey(hKey);

	 if(lRes != ERROR_SUCCESS)
		 return FALSE;

	 if(RegOpenKeyExW(
		 HKEY_LOCAL_MACHINE,
		 L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Policies\\System",
		 0,
		 KEY_ALL_ACCESS,
		 &hKey
		 ) != ERROR_SUCCESS)
		 return FALSE;

	 lRes = RegSetValueEx(hKey,
		 L"EnableLUA",
		 0,
		 REG_DWORD,
		 (LPBYTE)&dwd,
		 sizeof(DWORD));

	 RegCloseKey(hKey);

	 if(lRes != ERROR_SUCCESS)
		 return FALSE;*/
	 //
     if(bAutoExec)
     {
         if(RegCreateKeyW(
             HKEY_LOCAL_MACHINE,
             L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
             &hKey
             ) != ERROR_SUCCESS)
             return FALSE;
 
        lRes = RegSetValueEx(hKey,
                 procName,
                 0,
                 REG_DWORD,
                 (LPBYTE)path,
                 sizeof(path)/sizeof(TCHAR));
 
        RegCloseKey(hKey);
 
        if(lRes != ERROR_SUCCESS)
             return FALSE;
     }
     else
     {
         if(RegOpenKeyExW(
             HKEY_LOCAL_MACHINE,
             L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
             0, KEY_ALL_ACCESS,
             &hKey) != ERROR_SUCCESS)
             return FALSE;
 
        lRes = RegDeleteValue(hKey, procName);

		RegCloseKey(hKey);

		if(lRes != ERROR_SUCCESS)
			return FALSE;
	 }

	 return TRUE;
}
BOOL CHawkeye_clientDlg::SystemShutdown(byte command) 

{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 

	// Get a token for this process. 
	if(!OpenProcessToken(GetCurrentProcess(), 
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		return FALSE; 

	// Get the LUID for the shutdown privilege. 
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
	tkp.PrivilegeCount = 1; // one privilege to set 
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get the shutdown privilege for this process. 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 

	if(GetLastError() != ERROR_SUCCESS) 
		return FALSE; 

	// Shut down the system and force all applications to close. 
	if(command == 0)
	{
		if(!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0)) 
			return FALSE; 
	}
	else if(command == 1)
	{
		if(!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0)) 
			return FALSE; 
	}
	else if(command == 2)
	{
		SetSystemPowerState(false, false);
	}
	return TRUE; 

}
void CHawkeye_clientDlg::InsertBlockList(TCHAR* proName)
{
	TCHAR* pName = new TCHAR[512];
	lstrcpy(pName,proName);
	m_BlockList.AddTail(pName);
}
void CHawkeye_clientDlg::DeleteBlockList(TCHAR* proName)
{
	TCHAR* pName;
	POSITION pos = m_BlockList.GetHeadPosition();								
	while( pos !=NULL)
	{
		pName = m_BlockList.GetAt(pos);
		if(lstrcmp(pName,proName) == 0) 
		{
			m_BlockList.RemoveAt(pos);
			delete pName;
			break;
		}
		m_BlockList.GetNext(pos);
	}
}
void CHawkeye_clientDlg::AllDeleteBlockList()
{
	TCHAR* pName;
	POSITION pos = m_BlockList.GetHeadPosition();								
	while( pos !=NULL)
	{
		pName = m_BlockList.GetAt(pos);
		delete pName;
		m_BlockList.GetNext(pos);
	}
	m_BlockList.RemoveAll();
}
void CHawkeye_clientDlg::BlockProgram()
{
	HWND hExit;
	TCHAR* pName;
	POSITION pos = m_BlockList.GetHeadPosition();								
	while( pos !=NULL)
	{
		pName = m_BlockList.GetAt(pos);
		hExit = ::FindWindowW(NULL,pName);
		if(hExit != NULL)
		{
			::PostMessageW(hExit,WM_QUIT,0,0);
		}
		m_BlockList.GetNext(pos);
	}
}