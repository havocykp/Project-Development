// HeapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "HeapDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

// CHeapDlg 对话框

IMPLEMENT_DYNAMIC(CHeapDlg, CDialogEx)

CHeapDlg::CHeapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_HEAP, pParent)
{

}

CHeapDlg::~CHeapDlg()
{
}

void CHeapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST_HEAP, m_HeapList);
	DDX_Control(pDX, IDC_LIST_HEAP, m_HeapList);
}


BEGIN_MESSAGE_MAP(CHeapDlg, CDialogEx)
END_MESSAGE_MAP()


// CHeapDlg 消息处理程序


BOOL CHeapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//typedef struct tagHEAPLIST32
	//{
	//	SIZE_T dwSize;
	//	DWORD  th32ProcessID;   // owning process
	//	ULONG_PTR  th32HeapID;      // heap (in owning process's context!)
	//	DWORD  dwFlags;
	//} HEAPLIST32;
	HEAPLIST32  hl32 = { sizeof(HEAPLIST32) };
	//he32.dwSize = sizeof(HEAPLIST32);//no use

	m_HeapList.InsertColumnCustom(4, 0.2, L"PID", 0.2, L"HeapID", 0.2, L"Size", 0.2, L"Flags");

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, m_pid);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		MessageBox(L"无法获取此进程的堆快照");
		return TRUE;
	}
	if (!Heap32ListFirst(hSnapshot, &hl32)) {
		MessageBox(L"无法获取堆快照的首个堆信息");
		return TRUE;
	}

	INT loop = 0;
	do {
		CString strPID, strHeapID, strSize, strFlags;
		strPID.Format(L"%d", hl32.th32ProcessID);
		strHeapID.Format(L"%d", hl32.th32HeapID);
		strSize.Format(L"%d", hl32.dwSize);
		strFlags.Format(L"%d", hl32.dwFlags);
		m_HeapList.InsertItemCustom(4, strPID, strHeapID, strSize, strFlags);
		if (loop > 20) {
			//break;
		}
		loop++;
	} while (Heap32ListNext(hSnapshot, &hl32));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
