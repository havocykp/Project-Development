#pragma once
#include "FilesTree.h"
#include "afxeditbrowsectrl.h"


// CFileDlg 对话框

class CFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileDlg)

public:
	CFileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void TraverseFiles(CString fullPath, HTREEITEM  hTreeNode);
	void TraverseDeleteFiles(CString fullPath, CString postFix);
public:
	CFilesTree m_FilesTree;
	CString m_EditFilePostfix;
	CMFCEditBrowseCtrl m_FileBrowser;
	afx_msg void OnEnChangeFileBrowser();
	afx_msg void OnBnClickedButtonClearFile();
};
