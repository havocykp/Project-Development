#pragma once


// CDialogAnnoymous 对话框

class CDialogAnnoymous : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogAnnoymous)

public:
	CDialogAnnoymous(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogAnnoymous();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANNOYMOUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
