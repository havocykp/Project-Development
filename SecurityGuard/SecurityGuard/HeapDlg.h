#pragma once
#include "afxcmn.h"
#include "HeapList.h"


// CHeapDlg 对话框

class CHeapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHeapDlg)

public:
	CHeapDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHeapDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HEAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	CListCtrl m_HeapList;
	INT m_pid;
	CHeapList m_HeapList;
	virtual BOOL OnInitDialog();
};
