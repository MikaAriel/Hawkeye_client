
// Hawkeye_clientDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "TrayIconMng.h"
#include "DriverMgr.h"

// CHawkeye_clientDlg ��ȭ ����
class CHawkeye_clientDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CHawkeye_clientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	virtual ~CHawkeye_clientDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_HAWKEYE_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	// Ʈ���� ������ ������ ��ü
	CTrayIconMng m_myTray;
	// Ʈ���� ������ ���̱�/����� ����
	bool m_first;
	bool m_bHide;
	long OnTrayIcon(WPARAM wParam, LPARAM lParam);
	void OnAppExit(void);
	void OnDialogShow(void);
public:
	//�޼����� �ڵ�
	afx_msg void OnTimer(UINT nIDEvent);
	void CheckTopWindowTimer();
	TCHAR tpProcess[512];
	DWORD lastTime;
public:
	BOOL SetRegistyStartProgram(BOOL bAutoExec);
	bool m_againreg;
public:
	//��Ÿ��ɵ�
	BOOL SystemShutdown(byte command);
	//���α׷����
	void InsertBlockList(TCHAR* proName);
	void DeleteBlockList(TCHAR* proName);
	void AllDeleteBlockList();
	void BlockProgram();
	CList<TCHAR*> m_BlockList;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_logo;
	CListCtrl m_listlog;
	DriverMgr drvmgr;
};
