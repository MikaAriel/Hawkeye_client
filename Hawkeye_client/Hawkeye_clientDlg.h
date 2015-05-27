
// Hawkeye_clientDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "TrayIconMng.h"
#include "DriverMgr.h"

// CHawkeye_clientDlg 대화 상자
class CHawkeye_clientDlg : public CDialogEx
{
// 생성입니다.
public:
	CHawkeye_clientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	virtual ~CHawkeye_clientDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_HAWKEYE_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	// 트레이 아이콘 관리용 객체
	CTrayIconMng m_myTray;
	// 트레이 아이콘 보이기/숨기기 여부
	bool m_first;
	bool m_bHide;
	long OnTrayIcon(WPARAM wParam, LPARAM lParam);
	void OnAppExit(void);
	void OnDialogShow(void);
public:
	//메세지훅 코드
	afx_msg void OnTimer(UINT nIDEvent);
	void CheckTopWindowTimer();
	TCHAR tpProcess[512];
	DWORD lastTime;
public:
	BOOL SetRegistyStartProgram(BOOL bAutoExec);
	bool m_againreg;
public:
	//기타명령들
	BOOL SystemShutdown(byte command);
	//프로그램블록
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
