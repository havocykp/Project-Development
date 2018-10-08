// PowerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "PowerDlg.h"
#include "afxdialogex.h"
#include <PowrProf.h>
#pragma comment(lib,"PowrProf.lib")


// CPowerDlg 对话框

IMPLEMENT_DYNAMIC(CPowerDlg, CDialogEx)

CPowerDlg::CPowerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_POWER, pParent)
{

}

CPowerDlg::~CPowerDlg()
{
}

void CPowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPowerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_POWER_OFF, &CPowerDlg::OnBnClickedButtonPowerOff)
	ON_BN_CLICKED(IDC_BUTTON_RESTART, &CPowerDlg::OnBnClickedButtonRestart)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CPowerDlg::OnBnClickedButtonLogout)
	ON_BN_CLICKED(IDC_BUTTON_LOCK, &CPowerDlg::OnBnClickedButtonLock)
	ON_BN_CLICKED(IDC_BUTTON_SLEEP, &CPowerDlg::OnBnClickedButtonSleep)
	ON_BN_CLICKED(IDC_BUTTON_REST, &CPowerDlg::OnBnClickedButtonRest)
	ON_BN_CLICKED(IDC_BUTTON_HOT_KEY, &CPowerDlg::OnBnClickedButtonHotKey)
END_MESSAGE_MAP()


// CPowerDlg 消息处理程序

BOOL CPowerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//开启开机特权
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//调用函数提升权限
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CPowerDlg::OnBnClickedButtonPowerOff()
{
	// TODO: 在此添加控件通知处理程序代码
	ExitWindowsEx(EWX_POWEROFF, EWX_FORCE);
}





void CPowerDlg::OnBnClickedButtonRestart()
{
	// TODO: 在此添加控件通知处理程序代码
	ExitWindowsEx(EWX_REBOOT, EWX_FORCE);
}




void CPowerDlg::OnBnClickedButtonLogout()
{
	// TODO: 在此添加控件通知处理程序代码
	ExitWindowsEx(EWX_LOGOFF, EWX_FORCE);
}




void CPowerDlg::OnBnClickedButtonLock()
{
	// TODO: 在此添加控件通知处理程序代码
	LockWorkStation();
}


void CPowerDlg::OnBnClickedButtonSleep()
{
	// TODO: 在此添加控件通知处理程序代码
	SetSuspendState(TRUE, FALSE, FALSE);
}


void CPowerDlg::OnBnClickedButtonRest()
{
	// TODO: 在此添加控件通知处理程序代码
	SetSuspendState(TRUE, FALSE, FALSE);
}




BOOL CPowerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_HOTKEY && pMsg->wParam == 0xa001) {
		MessageBox(L"隐藏");
		if (m_IsWindowHide == TRUE) {
			ShowWindow(SW_HIDE);
			m_IsWindowHide = FALSE;
		}
		else {
			ShowWindow(SW_SHOW);
			m_IsWindowHide = TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CPowerDlg::OnBnClickedButtonHotKey()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL res = ::RegisterHotKey(this->GetSafeHwnd(), 0xa001, MOD_CONTROL | MOD_SHIFT, 'K');
	if (res) {
		MessageBox(L"注册成功");
	}
	else {
		MessageBox(L"注册失败");
	}
}
