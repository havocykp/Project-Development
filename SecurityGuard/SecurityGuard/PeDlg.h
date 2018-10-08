#pragma once
#include "afxeditbrowsectrl.h"
#include "Pe.h"

// CPeDlg 对话框

class CPeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPeDlg)

public:
	CPeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOADPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMFCEditBrowseCtrl m_EditBrowseCtrl;
	afx_msg void OnEnChangeMfceditbrowsePeFile();
	CString m_strChoosedFile;
	afx_msg void OnBnClickedButtonLoadPe();
	CPe*  m_pPE;
};
