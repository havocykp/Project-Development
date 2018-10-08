#pragma once
#include "SectionHeaderList.h"
#include "Pe.h"

// CSectionHeaderDlg 对话框

class CSectionHeaderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSectionHeaderDlg)

public:
	CSectionHeaderDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSectionHeaderDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SectionHeader };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void ShowSectionHeaderInfo();
public:
	CPe* m_pPE;
	CSectionHeaderList m_SectionHeaderList;
	virtual BOOL OnInitDialog();
};
