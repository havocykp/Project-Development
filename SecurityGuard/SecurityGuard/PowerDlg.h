#pragma once


// CPowerDlg 对话框

class CPowerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPowerDlg)

public:
	CPowerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPowerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_POWER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPowerOff();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRestart();
	afx_msg void OnBnClickedButtonLogout();
	afx_msg void OnBnClickedButtonLock();
	afx_msg void OnBnClickedButtonSleep();
	afx_msg void OnBnClickedButtonRest();
	afx_msg void OnBnClickedButtonHotKey();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	BOOL m_IsWindowHide = TRUE;  // 全局热键使用
};
