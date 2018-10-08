// ProcessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "ProcessDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// CProcessDlg 对话框

IMPLEMENT_DYNAMIC(CProcessDlg, CDialogEx)

CProcessDlg::CProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PROCESS, pParent)
{

}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_ProcessList);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序


BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//添加进程遍历数据
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		MessageBox(L"无法获取进程快照");
		return TRUE;
	}
	PROCESSENTRY32  pe32 = { sizeof(PROCESSENTRY32) };
	if (Process32First(hSnapshot, &pe32) != TRUE) {
		MessageBox(L"无法获取进程快照中的首个进程");
		CloseHandle(hSnapshot);
		return TRUE;
	}
	//m_ProcessList.InsertColumnCustom(10, 0.1, L"PID", 0.2, L"exeFile", 0.1, L"cntThreads", 0.1, L"Size", 0.08, L"PriClassBase", 0.08, L"ParentPID", 0.08, L"MoudleID", 0.05, L"Usage", 0.05, L"Flags", 0.1, L"defHeadID");
	m_ProcessList.InsertColumnCustom(7, 0.1, L"PID",0.1, L"PIDhex", 0.2, L"exeFile", 0.1, L"cntThreads", 0.1, L"Size", 0.12, L"PriClassBase", 0.08, L"ParentPID");
	do {

		CString strPID, strMID, strPPID, strExeFile, strThreads, strUsage, strPriClassBase, strFlags, strSize, strHeadID;
		CString strPIDhex;
		strPIDhex.Format(L"%X", pe32.th32ProcessID);
		strPID.Format(L"%d", pe32.th32ProcessID);
		strPPID.Format(L"%d", pe32.th32ParentProcessID);
		strMID.Format(L"%d", pe32.th32ModuleID);
		strThreads.Format(L"%d", pe32.cntThreads);
		strUsage.Format(L"%d", pe32.cntUsage);

		strPriClassBase.Format(L"%d", pe32.pcPriClassBase);
		strHeadID.Format(L"%d", pe32.th32DefaultHeapID);
		strFlags.Format(L"%d", pe32.dwFlags);
		strSize.Format(L"%d", pe32.dwSize);

		strExeFile = pe32.szExeFile;

		//m_ProcessList.InsertItemCustom(10, strPID, strExeFile, strThreads, strSize, strPriClassBase, strPPID, strMID, strUsage, strFlags, strHeadID);
		m_ProcessList.InsertItemCustom(7, strPID,strPIDhex ,strExeFile, strThreads, strSize, strPriClassBase, strPPID);

	} while (Process32Next(hSnapshot, &pe32));


	CloseHandle(hSnapshot);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
