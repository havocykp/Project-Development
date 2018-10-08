#pragma once


// CInjectDlg 对话框

class CInjectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInjectDlg)

public:
	CInjectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInjectDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
