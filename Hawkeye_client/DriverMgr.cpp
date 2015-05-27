#include "StdAfx.h"
#include "DriverMgr.h"
#undef UNICODE
#undef _UNICODE

DriverMgr::DriverMgr()
{
	scm = NULL;
	strcpy(FullDeviceName,"\\\\.\\ddkhook\\PSNUM");
	strcpy(DriverName,"ddkhook.sys");
	bInstall = FALSE;
	bStart = FALSE;
	GetCurrentDirectoryA(_MAX_PATH,DriverExe);
	strcat(DriverExe, "\\ddkhook.sys");
}


DriverMgr::~DriverMgr(void)
{
}
BOOL DriverMgr::DriverInstall() {
	if(scm==NULL) scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	SC_HANDLE Service=CreateServiceA(scm,DriverName,DriverName,SERVICE_ALL_ACCESS,SERVICE_KERNEL_DRIVER,SERVICE_DEMAND_START,SERVICE_ERROR_NORMAL,DriverExe,NULL,NULL,NULL,NULL,NULL);
	if (Service==NULL) {
		DWORD err=GetLastError();
		if(err==ERROR_SERVICE_EXISTS) {
			return TRUE;
		}
		else {
			/*char str[64];
			sprintf_s(str,"%s=%d","ERR: Can't stop service. Err",err);
			MessageBox(NULL,str,"Error",MB_OK);*/
			CString dtime;
			_wstrtime((TCHAR*)dtime.GetString());
			plog->InsertItem(0,dtime);
			plog->SetItemText(0,1,L"ERR");
			plog->SetItemText(0,2,L"Install Service");
			plog->UpdateData(false);
		}
		return FALSE;
	}
	CloseServiceHandle(Service);
	return TRUE;
}
BOOL DriverMgr::DriverRemove() {
	if(scm==NULL) scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

	SC_HANDLE Service=OpenServiceA(scm,DriverName,SERVICE_ALL_ACCESS);
	if(Service==NULL) {
		CloseServiceHandle(scm);
		scm=NULL;
		return FALSE;
	}
	BOOL ret=DeleteService(Service);
	if(!ret) {
		/*char str[64];
		sprintf_s(str,"%s=%d","ERR: Can't stop service. Err",GetLastError());
		MessageBox(NULL,str,"Error",MB_OK);*/
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		plog->InsertItem(0,dtime);
		plog->SetItemText(0,1,L"ERR");
		plog->SetItemText(0,2,L"Delete Service");
		plog->UpdateData(false);
		return FALSE;
	} 
	CloseServiceHandle(Service);
	return TRUE;
}
BOOL DriverMgr::DriverStart() {
	if(scm==NULL) scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

	SC_HANDLE Service=OpenServiceA(scm,DriverName,SERVICE_ALL_ACCESS);
	if(Service==NULL) {
		CloseServiceHandle(scm);
		scm=NULL;
		return FALSE;
	}
	BOOL ret=StartService(Service,0,NULL);
	if(!ret) { 
		DWORD err=GetLastError();
		if(err==ERROR_SERVICE_ALREADY_RUNNING) {
			ret=TRUE;
		}
		else { 
			/*char str[64];
			sprintf_s(str,"%s=%d","ERR: Can't stop service. Err",err);
			MessageBox(NULL,str,"Error",MB_OK);*/
			CString dtime;
			_wstrtime((TCHAR*)dtime.GetString());
			plog->InsertItem(0,dtime);
			plog->SetItemText(0,1,L"ERR");
			plog->SetItemText(0,2,L"Start Service");
			plog->UpdateData(false);
			return FALSE;
		}
	}
	CloseServiceHandle(Service);
	return TRUE;
}
BOOL DriverMgr::DriverStop() {
	if(scm==NULL) scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

	SC_HANDLE Service=OpenServiceA(scm,DriverName,SERVICE_ALL_ACCESS);
	if(Service==NULL) {
		DWORD err=GetLastError();
		CloseServiceHandle(scm);
		scm=NULL;
		return FALSE;
	}
	SERVICE_STATUS serviceStatus;
	BOOL ret=ControlService(Service,SERVICE_CONTROL_STOP,&serviceStatus);
	if(!ret) {
		/*char str[64];
		sprintf_s(str,"%s=%d","ERR: Can't stop service. Err",GetLastError());
		MessageBox(NULL,str,"Error",MB_OK);*/
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		plog->InsertItem(0,dtime);
		plog->SetItemText(0,1,L"ERR");
		plog->SetItemText(0,2,L"Stop Service");
		plog->UpdateData(false);
		return FALSE;
	}

	CloseServiceHandle(Service);
	return TRUE;
}
void DriverMgr::SetListPointer(CListCtrl* log)
{
	plog = log;
}
BOOL DriverMgr::InitDriver()
{
	BOOL RetBOOL;
	RetBOOL=DriverInstall();
	if(!RetBOOL) 
	{
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		plog->InsertItem(0,dtime);
		plog->SetItemText(0,1,L"ERR");
		plog->SetItemText(0,2,L"Cannot Install");
		plog->UpdateData(false);
		return FALSE;
	}
	else {
		bInstall=TRUE;
		RetBOOL=DriverStart();
		if(!RetBOOL) 
		{
			CString dtime;
			_wstrtime((TCHAR*)dtime.GetString());
			plog->InsertItem(0,dtime);
			plog->SetItemText(0,1,L"ERR");
			plog->SetItemText(0,2,L"Cannot Start");
			plog->UpdateData(false);
			return FALSE;
		}
		else {
			bStart=TRUE;
			CString dtime;
			_wstrtime((TCHAR*)dtime.GetString());
			plog->InsertItem(0,dtime);
			plog->SetItemText(0,1,L"DRIVER");
			plog->SetItemText(0,2,L"Start Success");
			plog->UpdateData(false);
		}
	}
	return TRUE;
}
BOOL DriverMgr::RemoveDriver()
{
	BOOL RetBOOL;
	if(bInstall) {
		RetBOOL=DriverStop();
		if(!RetBOOL) 
		{
			CString dtime;
			_wstrtime((TCHAR*)dtime.GetString());
			plog->InsertItem(0,dtime);
			plog->SetItemText(0,1,L"ERR");
			plog->SetItemText(0,2,L"Cannot Stop");
			plog->UpdateData(false);
			return FALSE;
		}
		else {
			bStart=FALSE;
			RetBOOL=DriverRemove();
			if(!RetBOOL) 
			{
				CString dtime;
				_wstrtime((TCHAR*)dtime.GetString());
				plog->InsertItem(0,dtime);
				plog->SetItemText(0,1,L"ERR");
				plog->SetItemText(0,2,L"Cannot Remove");
				plog->UpdateData(false);
				return FALSE;
			}
			else {
				bInstall=FALSE;
				CString dtime;
				_wstrtime((TCHAR*)dtime.GetString());
				plog->InsertItem(0,dtime);
				plog->SetItemText(0,1,L"DRIVER");
				plog->SetItemText(0,2,L"Remove Success");
				plog->UpdateData(false);
				return TRUE;
			}
		}
	}
	else 
	{
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		plog->InsertItem(0,dtime);
		plog->SetItemText(0,1,L"ERR");
		plog->SetItemText(0,2,L"Cannot Remove");
		plog->UpdateData(false);
		return FALSE;
	}
	return TRUE;
}
void DriverMgr::GetMyCode()
{
	HANDLE hSnapshot;
	PROCESSENTRY32 pEntry;
	char flag;
	int ListCnt=0;

	hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPALL,NULL);
	pEntry.dwSize=sizeof(pEntry);
	flag=Process32First(hSnapshot,&pEntry);

	while(flag) {
		if(lstrcmp(pEntry.szExeFile,L"Hawkeye_client.exe") == 0)
		{
			sprintf_s(MyID,"%d",pEntry.th32ProcessID);
			CString dtime;
			_wstrtime((TCHAR*)dtime.GetString());
			plog->InsertItem(0,dtime);
			plog->SetItemText(0,1,L"FIND");
			plog->SetItemText(0,2,L"ID Find Success");
			plog->UpdateData(false);
		}
		flag=Process32Next(hSnapshot,&pEntry);
	}
	CloseHandle(hSnapshot);
}
void DriverMgr::ActivateBlock()
{
	ULONG ret;
	handle=CreateFileA(FullDeviceName,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(handle==(HANDLE)-1) {
		CString dtime;
		_wstrtime((TCHAR*)dtime.GetString());
		plog->InsertItem(0,dtime);
		plog->SetItemText(0,1,L"ERR");
		plog->SetItemText(0,2,L"Cannot Activate");
		plog->UpdateData(false);
		return;
	}
	WriteFile(handle,MyID,(DWORD)strlen(MyID),&ret,NULL);
	CloseHandle(handle);
	CString dtime;
	_wstrtime((TCHAR*)dtime.GetString());
	plog->InsertItem(0,dtime);
	plog->SetItemText(0,1,L"DRIVER");
	plog->SetItemText(0,2,L"Activate Success");

	plog->InsertItem(0,dtime);
	plog->SetItemText(0,1,L"시작");
	plog->SetItemText(0,2,L"[강제종료저항]");
	plog->UpdateData(false);
}