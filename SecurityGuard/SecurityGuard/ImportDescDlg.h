#pragma once
#include "Pe.h"
#include "ImportDllList.h"
#include "ImportThunkList.h"


// CImportDescDlg 对话框

class CImportDescDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportDescDlg)

public:
	CImportDescDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImportDescDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Import_desc };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void ShowImportDllList();
	void ShowImportThunkByDll();

public:
	CPe* m_pPE;

	CImportDllList m_ImportDllList;
	CImportThunkList m_ImportThunkList;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListDll(NMHDR *pNMHDR, LRESULT *pResult);
};
