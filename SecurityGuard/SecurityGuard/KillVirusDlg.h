#pragma once
#include "ClientSocket.h"
#include "afxeditbrowsectrl.h"

// CKillVirusDlg 对话框

class CKillVirusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKillVirusDlg)

public:
	CKillVirusDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKillVirusDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_KillVirus };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnKillvirusonline();
	void CheckSingleFileOnLine();
	void CheckPathOnLine();
	void TraverseCheckFilesOnLine(CString fullPath);

	void CheckVirusOffLine(int type);
	void TraverseCheckFilesOffLine(CString fullPath, int type);
	void CheckPathOffLine(int type);
	void CheckSingleFileOffLine(int type);
	void CheckProcessOffLine();

public:
	CClientSocket m_client;
//	CMFCEditBrowseCtrl m_BrowserPath;
//	CMFCEditBrowseCtrl m_BrowserFile;
	afx_msg void OnEnChangeMfceditbrowsePath();
	afx_msg void OnEnChangeMfceditbrowseFile();
	CString m_BrowserPath;
	CString m_BrowserFile;
	afx_msg void OnBnClickedButtonmd5save();
//	afx_msg void OnBnClickedButtonSavePathAndMd5();
//	afx_msg void OnBnClickedButtonProcessSave();
	afx_msg void OnBnClickedButtonkillByMd5();
	afx_msg void OnBnClickedButtonkillByPath();
	afx_msg void OnBnClickedButtonkillByProcess();

public:
	WATCHINFO* m_pVirusBase;
	int m_nVirtusBaseCount;
};
