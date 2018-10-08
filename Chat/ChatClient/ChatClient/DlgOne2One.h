#pragma once


// CDlgOne2One 对话框
#include "DlgChatMain.h"

class CDlgOne2One : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOne2One)

public:
	CDlgOne2One(/*SOCKET sClient ,*/CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgOne2One();

// 对话框数据
	enum { IDD = IDD_DLGONE2ONE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strShow;       // 聊天编辑框	
	CString m_strSend;       // 发送编辑框
	CString m_strTitle;      // 标题
	SOCKET m_sClient;
	CString m_FilePath;
	HANDLE fileThread;
	afx_msg void OnBtnProcSend();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonSendfile();
	
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
