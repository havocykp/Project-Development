#pragma once


// CAllInject 对话框

class CAllInject : public CDialogEx
{
	DECLARE_DYNAMIC(CAllInject)

public:
	CAllInject(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAllInject();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Inject };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
