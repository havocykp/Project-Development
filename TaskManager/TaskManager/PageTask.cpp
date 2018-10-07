// PageTask.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "PageTask.h"
#include "afxdialogex.h"


// CPageTask 对话框
HWND hwnd;

IMPLEMENT_DYNAMIC(CPageTask, CDialogEx)

CPageTask::CPageTask(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PAGE_TASK, pParent)
{

}

CPageTask::~CPageTask()
{
}

void CPageTask::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TASK, m_Task);
}


BEGIN_MESSAGE_MAP(CPageTask, CDialogEx)
END_MESSAGE_MAP()


// CPageTask 消息处理程序

// 应用程序
BOOL CPageTask::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
