
// TaskManagerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "PageChart.h"
#include "PageProc.h"
#include "PageTask.h"


// CTaskManagerDlg 对话框
class CTaskManagerDlg : public CDialogEx
{
// 构造
public:
	CTaskManagerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASKMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabCtrl;
	CPageProc m_proc;
	CPageTask m_task;
	CPageChart m_chart;
#define MAX_DLG_PAGE 5
	CDialog *m_pDialog[MAX_DLG_PAGE];
	int m_CurSelTab;
	afx_msg void OnTcnSelchangeTabManage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnTrayRestore();
	afx_msg void OnTrayExit();

};
