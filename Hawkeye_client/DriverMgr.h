#pragma once

class DriverMgr
{
public:
	DriverMgr();
	virtual ~DriverMgr(void);
public:
	void SetListPointer(CListCtrl* log);
	BOOL InitDriver();
	BOOL RemoveDriver();
	void GetMyCode();
	void ActivateBlock();
private:
	BOOL DriverInstall();
	BOOL DriverRemove();
	BOOL DriverStart();
	BOOL DriverStop();
public:
	SC_HANDLE scm;
	HANDLE handle;
	char MyID[10];
	CListCtrl* plog;
	char FullDeviceName[100];
	char DriverName[15];
	char DriverExe[_MAX_PATH];
	BOOL bInstall,bStart;
};

