#pragma once
#include "afxcmn.h"


// CPageTask 对话框

class CPageTask : public CDialogEx
{
	DECLARE_DYNAMIC(CPageTask)

public:
	CPageTask(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageTask();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_TASK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Task;
	virtual BOOL OnInitDialog();
};

