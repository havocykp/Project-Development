#pragma once
#include "afxcmn.h"
#include "ModuleList.h"


// CModuleDlg 对话框

class CModuleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModuleDlg)

public:
	CModuleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModuleDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	CListCtrl m_ModuleList;
	INT m_pid;
//	virtual void PreInitDialog();
	virtual BOOL OnInitDialog();
	CModuleList m_ModuleList;
};
