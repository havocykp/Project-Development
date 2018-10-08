#pragma once
#include "afxcmn.h"


// CDlgSearch 对话框

class CDlgSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSearch)

public:
	CDlgSearch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSearch();

// 对话框数据
	enum { IDD = IDD_DIALOGSEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//void InsertFriend(CHATUPDATEUSER& objUpdate);
	CString m_strSearch;
	afx_msg void OnBnClickedOk();
	CListCtrl m_listFriend;
};
