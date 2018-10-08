// DlgRecord.cpp : 实现文件
//

#include "stdafx.h"
#include "Chatclient.h"
#include "DlgRecord.h"
#include "afxdialogex.h"
#include "DlgChatMain.h"

// CDlgRecord 对话框

IMPLEMENT_DYNAMIC(CDlgRecord, CDialogEx)

CDlgRecord::CDlgRecord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRecord::IDD, pParent)
{

}

CDlgRecord::~CDlgRecord()
{
}

void CDlgRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Record, m_listRecord);
}


BEGIN_MESSAGE_MAP(CDlgRecord, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgRecord 消息处理程序


BOOL CDlgRecord::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listRecord.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listRecord.InsertColumn(0, L"from", 0, 50);
	m_listRecord.InsertColumn(1, L"to", 0, 50);
	m_listRecord.InsertColumn(2, L"content", 0, 300);
	UpdateList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDlgRecord::UpdateList()
{
	// 删除之前的聊天记录
	m_listRecord.DeleteAllItems();
	CDlgChatMain* pParent = (CDlgChatMain*)GetParent();
	auto& vec = pParent->m_pClient->m_vecMsgRecord;
	DWORD dwCount = vec.size();
	CString strTemp;
	for (DWORD i = 0; i < dwCount; i++)
	{
		// 聊天发起方
		strTemp = vec[i].szFrom;
		m_listRecord.InsertItem(i, strTemp.GetBuffer());
		// 聊天接收方
		strTemp = vec[i].szTo;
		m_listRecord.SetItemText(i, 1, strTemp.GetBuffer());
		// 聊天内容
		strTemp = vec[i].szContent;
		m_listRecord.SetItemText(i, 2, strTemp.GetBuffer());
	}
}

void CDlgRecord::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}
