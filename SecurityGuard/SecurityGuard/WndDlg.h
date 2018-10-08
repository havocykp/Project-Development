#pragma once
#include "WndList.h"


// CWndDlg 对话框

class CWndDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWndDlg)

public:
	CWndDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWndDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CWndList m_WndList;
	virtual BOOL OnInitDialog();
};
