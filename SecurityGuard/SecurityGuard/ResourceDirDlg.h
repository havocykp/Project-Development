#pragma once
#include "Pe.h"
#include "afxcmn.h"


// CResourceDirDlg 对话框

class CResourceDirDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResourceDirDlg)

public:
	CResourceDirDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResourceDirDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Resource };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowRescInfo();
	void UpdateByClick(CString strLay1, CString strLay2);
public:
	CPe* m_pPE;
	CString m_RootNumOfNamedEntries;
	CString m_RootNumOfIdEntries;
	CString m_NumOfNamedEntries2;
	CString m_NumOfIdEntries2;
	CString m_3LayerRVA;
	CString m_3LayerOffset;
	CString m_3LayerSize;

	CTreeCtrl m_RescTreeCtrl;
	
	afx_msg void OnTvnSelchangedTreeResource(NMHDR *pNMHDR, LRESULT *pResult);
};
