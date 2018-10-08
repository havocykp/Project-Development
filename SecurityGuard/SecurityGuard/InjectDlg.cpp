// InjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "InjectDlg.h"
#include "afxdialogex.h"


// CInjectDlg 对话框

IMPLEMENT_DYNAMIC(CInjectDlg, CDialogEx)

CInjectDlg::CInjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_INJECT, pParent)
{

}

CInjectDlg::~CInjectDlg()
{
}

void CInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInjectDlg, CDialogEx)
END_MESSAGE_MAP()


// CInjectDlg 消息处理程序
