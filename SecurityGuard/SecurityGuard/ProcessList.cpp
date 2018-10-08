// ProcessList.cpp : 实现文件
//

#include "stdafx.h"
#include <windows.h>
#include "SecurityGuard.h"
#include "ProcessList.h"


// CProcessList

IMPLEMENT_DYNAMIC(CProcessList, CListCtrl)

CProcessList::CProcessList()
{

}

CProcessList::~CProcessList()
{
}


BEGIN_MESSAGE_MAP(CProcessList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CProcessList::OnNMRClick)
	ON_COMMAND(ID_SHOWTHREAD, &CProcessList::OnShowThread)
	ON_COMMAND(ID_SHOW_MODULE, &CProcessList::OnShowModule)
	ON_COMMAND(ID_SHOW_HEAP, &CProcessList::OnShowHeap)
	ON_COMMAND(ID_TERMINATE_PROCESS, &CProcessList::OnTerminateProcess)
	ON_COMMAND(ID_Hook, &CProcessList::OnHook)
END_MESSAGE_MAP()

void CProcessList::InsertColumnCustom(DWORD dwCount, ...)
{
	//设置扩展样式
	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//获取尺寸
	CRect rc;
	GetClientRect(&rc);
	INT nTotalWidth = (INT)rc.Width();
	INT nUsedWidth = 0;
	INT nWidth; //每个column的宽度
	va_list list;
	va_start(list, dwCount);
	for (DWORD i = 0; i < dwCount; i++) {
		DOUBLE  fWidthRate = va_arg(list, DOUBLE);
		if (i != dwCount - 1) {
			nWidth = (INT)(nTotalWidth * fWidthRate);
		}
		else {
			nWidth = nTotalWidth - nUsedWidth; //最后列宽使用剩余的所有宽度
		}

		if (nWidth < 10) {
			MessageBox(L"Warning: 列表的宽度太小了\n");
		}

		WCHAR* wStr = va_arg(list, WCHAR*);

		InsertColumn(i, wStr, 0, nWidth);
		nUsedWidth += nWidth;
	}
	va_end(list);

	return;
}


void CProcessList::InsertItemCustom(DWORD dwCount, ...)
{
	va_list list;
	va_start(list, dwCount);
	//首行位置插入一行
	InsertItem(0, L"");
	for (DWORD i = 0; i < dwCount; i++) {
		WCHAR* wStr = va_arg(list, WCHAR*);
		SetItemText(0, i, wStr);
	}

	va_end(list);
	return;
}


// CProcessList 消息处理程序
void CProcessList::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//添加线程操作  
		VERIFY(menu.LoadMenu(IDR_MENU1));           //这里是我们在1中定义的MENU的文件名称  
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

		//下面的两行代码主要是为了后面的操作为准备的  
		//获取列表视图控件中第一个被选择项的位置    
		POSITION m_pstion = GetFirstSelectedItemPosition();
		//该函数获取由pos指定的列表项的索引，然后将pos设置为下一个位置的POSITION值  
		INT m_nIndex = GetNextSelectedItem(m_pstion);

		//获取PID
		int nColumn = 0; //pid所在的列
		CString strPID = GetItemText(m_nIndex, nColumn);
		//MessageBox(strPID);
		m_nChoosedPid = _wtoi(strPID);
	}
}


void CProcessList::OnShowThread()
{
	// TODO: 在此添加命令处理程序代码
	m_pThreadDialog = new CThreadDlg();

	m_pThreadDialog->m_pid = m_nChoosedPid;
	m_pThreadDialog->DoModal();
}


void CProcessList::OnShowModule()
{
	// TODO: 在此添加命令处理程序代码
	m_pModuleDialog = new CModuleDlg();

	m_pModuleDialog->m_pid = m_nChoosedPid;
	//m_pModuleDialog->DoModal();
	m_pModuleDialog->Create(IDD_DIALOG_MODULE, this);
	//m_pModuleDialog->SendMessage(WM_PID_CHOOSED, 0, 255); //test
	m_pModuleDialog->ShowWindow(SW_SHOW);
}



void CProcessList::OnShowHeap()
{
	// TODO: 在此添加命令处理程序代码
	m_pHeapDialog = new CHeapDlg();
	m_pHeapDialog->m_pid = m_nChoosedPid;
	m_pHeapDialog->DoModal();
}


void CProcessList::OnTerminateProcess()
{
	// TODO: 在此添加命令处理程序代码
	HANDLE  hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, m_nChoosedPid);
	if (hProcess == INVALID_HANDLE_VALUE) {
		MessageBox(L"无法获取进程句柄");
		return;
	}
	if (!TerminateProcess(hProcess, 0)) {
		MessageBox(L"终止进程失败");
	}
}

HANDLE GetThePidOfTargetProcess(HWND hwnd)
{
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS | PROCESS_CREATE_THREAD, 0, pid);
	return hProcess;
}

// 提升权限
void Up()
{
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tp;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;
	AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_ADJUST_PRIVILEGES), NULL, NULL);

}

// 进程注入
BOOL DoInject(char* DllPath, HANDLE hProcess)
{
	DWORD BufSize = strlen(DllPath) + 1;
	LPVOID AllocAddr = VirtualAllocEx(hProcess, NULL, BufSize, MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, AllocAddr, DllPath, BufSize, NULL);
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");
	HANDLE hRemoteThread;
	hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, pfnStartAddr, AllocAddr, 0, NULL);
	if (hRemoteThread)
	{
		MessageBox(NULL, TEXT("注入成功"), TEXT("提示"), MB_OK);
		return true;
	}
	else
	{
		MessageBox(NULL, TEXT("注入失败"), TEXT("提示"), MB_OK);
		return false;
	}
}

void CProcessList::OnHook()
{
	// TODO: 在此添加命令处理程序代码
	HWND hwnd = FindWindowExA(NULL, NULL, NULL, "安全卫士");
	Up();
	HANDLE hP = GetThePidOfTargetProcess(hwnd);
	// 开始注入
	// Dll路径
	DoInject("C:\\Users\\yakun\\Desktop\\dllmain\\dllmain.dll", hP);
}
