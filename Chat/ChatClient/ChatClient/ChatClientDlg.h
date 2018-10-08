
// ChatclientDlg.h : 头文件
//

#pragma once
#include "CclientSocket.h"
#include "DlgChatMain.h"

// CChatclientDlg 对话框
class CChatclientDlg : public CDialogEx
{
// 构造
public:
	CChatclientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHATCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CclientSocket m_client;
	CString m_strName;
	CString m_strPass;
	afx_msg void OnBtnProcAnony();
	afx_msg void OnBtnProcRegister();
	afx_msg void OnBtnProcLogin();
	bool m_bSavePwd;
	bool m_bAuto;
	CBitmap m_BKGround;
	afx_msg void OnBnClickedCheckSavepwd();
	afx_msg void OnBnClickedCheckAuto();
	afx_msg void OnClose();
	LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTrayRestore();
	afx_msg void OnTrayExit();
	//afx_msg void OnBnClickedButton1();
};
