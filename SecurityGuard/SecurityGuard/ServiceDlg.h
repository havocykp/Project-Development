#pragma once
#include "ServiceList.h"


// CServiceDlg 对话框

class CServiceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServiceDlg)

public:
	CServiceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServiceDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Service };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void UpdateServiceInfo();

public:
	CServiceList m_ServiceList;
//	afx_msg void OnRclickListService(NMHDR *pNMHDR, LRESULT *pResult);
};
