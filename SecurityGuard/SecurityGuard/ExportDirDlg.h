#pragma once
#include "afxwin.h"
#include "Pe.h"
#include "ExportDirList.h"

// CExportDirDlg 对话框

class CExportDirDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExportDirDlg)

public:
	CExportDirDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExportDirDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_EXPORT_DIR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowExportDirInfo();
public:
	CPe* m_pPE;

	CString m_strOffset;
	CString m_strCharacteristic;
	CString m_strBase;
	CString m_strName;
	CString m_strNumOfFunc;
	CString m_strNumOfNames;
	CString m_strAddrOfFunc;
	CString m_strAddrOfNames;
	CString m_strAddrOfOrdinals;
	CString m_strNameString;
	
	CExportDirList m_ExportDirList;
};
