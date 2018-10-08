// ModuleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "ModuleDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// CModuleDlg 对话框

IMPLEMENT_DYNAMIC(CModuleDlg, CDialogEx)

CModuleDlg::CModuleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MODULE, pParent)
{

}

CModuleDlg::~CModuleDlg()
{
}

void CModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST_MODULE, m_ModuleList);
	DDX_Control(pDX, IDC_LIST_MODULE, m_ModuleList);
}


BEGIN_MESSAGE_MAP(CModuleDlg, CDialogEx)
END_MESSAGE_MAP()


// CModuleDlg 消息处理程序


//void CModuleDlg::PreInitDialog()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	CDialogEx::PreInitDialog();
//}


BOOL CModuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//typedef struct tagMODULEENTRY32W
	//{
	//	DWORD   dwSize;
	//	DWORD   th32ModuleID;       // This module
	//	DWORD   th32ProcessID;      // owning process
	//	DWORD   GlblcntUsage;       // Global usage count on the module
	//	DWORD   ProccntUsage;       // Module usage count in th32ProcessID's context
	//	BYTE  * modBaseAddr;        // Base address of module in th32ProcessID's context
	//	DWORD   modBaseSize;        // Size in bytes of module starting at modBaseAddr
	//	HMODULE hModule;            // The hModule of this module in th32ProcessID's context
	//	WCHAR   szModule[MAX_MODULE_NAME32 + 1];
	//	WCHAR   szExePath[MAX_PATH];
	//} MODULEENTRY32W;
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	m_ModuleList.InsertColumnCustom(8, 0.08, L"PID", 0.07, L"ModuleID", 0.05, L"Size", 0.08, L"GlblcntUsage", 0.08, L"ProccntUsage", 0.08, L"modBaseSize", 0.15, L"Module", 0.1, L"ExePath");

	/*CString msg;
	msg.Format(L"进程id是: %d", m_pid);
	MessageBox(msg);*/
	//遍历模块数据
	HANDLE  hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_pid);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		MessageBox(L"权限不足,无法获取模块快照");
		return TRUE;
	}
	if (!Module32First(hSnapshot, &me32)) {
		MessageBox(L"无法获取模块快照的首条信息");
		CloseHandle(hSnapshot);
		return TRUE;
	}

	do {
		CString strPID, strMID, strSize, strGlbUsage, strProcUsage, strModBaseSize, strModule, strExePath;
		strPID.Format(L"%d", me32.th32ProcessID);
		strMID.Format(L"%d", me32.th32ModuleID);
		strSize.Format(L"%d", me32.dwSize);
		strGlbUsage.Format(L"%d", me32.GlblcntUsage);
		strProcUsage.Format(L"%d", me32.ProccntUsage);
		strModBaseSize.Format(L"%d", me32.modBaseSize);
		strModule.Format(L"%s", me32.szModule); //只有一个字符
		strExePath.Format(L"%s", me32.szExePath);

		m_ModuleList.InsertItemCustom(8, strPID, strMID, strSize, strGlbUsage, strProcUsage, strModBaseSize, strModule, strExePath);
	} while (Module32Next(hSnapshot, &me32));

	CloseHandle(hSnapshot);
	//SendMessage(WM_PID_CHOOSED, NULL, 255);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
