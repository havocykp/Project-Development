// ThreadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "ThreadDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// CThreadDlg 对话框

IMPLEMENT_DYNAMIC(CThreadDlg, CDialogEx)

CThreadDlg::CThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_THREAD, pParent)
{

}

CThreadDlg::~CThreadDlg()
{
}

void CThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_THREAD, m_ThreadList);
}


BEGIN_MESSAGE_MAP(CThreadDlg, CDialogEx)
END_MESSAGE_MAP()


// CThreadDlg 消息处理程序


BOOL CThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//创建线程列表
	//typedef struct tagTHREADENTRY32
	//{
	//	DWORD   dwSize;
	//	DWORD   cntUsage;
	//	DWORD   th32ThreadID;       // this thread
	//	DWORD   th32OwnerProcessID; // Process this thread is associated with
	//	LONG    tpBasePri;
	//	LONG    tpDeltaPri;
	//	DWORD   dwFlags;
	//} THREADENTRY32;
	m_ThreadList.InsertColumnCustom(7, 0.13, L"OwerPID", 0.13, L"ThreadID", 0.13, L"BasePri", 0.13, L"DeltaPri", 0.13, L"Size", 0.13, L"Flags", 0.13, L"Usage");

	//获取进行下的线程列表
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, m_pid);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		MessageBox(L"无法获取当前进程的线程快照");
		return TRUE;
	}
	THREADENTRY32 te32;
	te32.dwSize = sizeof(THREADENTRY32);
	if (!Thread32First(hSnapshot, &te32)) {
		MessageBox(L"无法获取当前线程快照中的首个线程信息");
		return TRUE;
	}
	CString  strTid, strPid, strBasePri, strDeltaPri, strSize, strFlags, strUsage;

	do {
		if (te32.th32OwnerProcessID == m_pid) {
			strTid.Format(L"%d", te32.th32ThreadID);
			strPid.Format(L"%d", te32.th32OwnerProcessID);
			strBasePri.Format(L"%d", te32.tpBasePri);
			strDeltaPri.Format(L"%d", te32.tpDeltaPri);
			strSize.Format(L"%d", te32.dwSize);
			strFlags.Format(L"%d", te32.dwFlags);
			strUsage.Format(L"%d", te32.cntUsage);
			m_ThreadList.InsertItemCustom(7, strPid, strTid, strBasePri, strDeltaPri, strSize, strFlags, strUsage);
		}
	} while (Thread32Next(hSnapshot, &te32));

	CloseHandle(hSnapshot);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
