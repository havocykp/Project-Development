// PageProc.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "PageProc.h"
#include "afxdialogex.h"
#include <Tlhelp32.h>
#include <Psapi.h>

#pragma comment(lib, "Psapi.lib")
// CPageProc 对话框

IMPLEMENT_DYNAMIC(CPageProc, CDialogEx)

CPageProc::CPageProc(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PAGE_PROCESS, pParent)
{

}

CPageProc::~CPageProc()
{
}

void CPageProc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROC, m_Proc);
}


BEGIN_MESSAGE_MAP(CPageProc, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROC, &CPageProc::OnNMRClickListProc)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PROC, &CPageProc::OnLvnColumnclickListProc)
END_MESSAGE_MAP()


// CPageProc 消息处理程序

// 排序相关代码
DWORD CPageProc::m_SortColum = 0;
BOOL CPageProc::m_bAs = TRUE;

int CALLBACK CPageProc::MyListCompar(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// 通过传递的参数来得到CSortList对象指针，从而得到排序方式
	CListCtrl* pListCtrl = (CListCtrl*)lParamSort;

	// 通过ItemData来确定数据
	int iCompRes;
	CString szComp1 = pListCtrl->GetItemText(lParam1, m_SortColum);
	CString szComp2 = pListCtrl->GetItemText(lParam2, m_SortColum);

	switch (m_SortColum)
	{
	case (2): case(3): case (4):
		// 以第一列为根据排序  编号
		// _ttol
		// atof
		iCompRes = _ttol(szComp1) <= _ttol(szComp2) ? -1 : 1; // 按照整数比较
		break;

	default:
		iCompRes = szComp1.Compare(szComp2); // 字符串比较
		break;
	}

	// 根据当前的排序方式进行调整
	if (m_bAs)
		return iCompRes;
	else
		return -iCompRes;
}

// 获取CPU使用率
double FILETIME2Double(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime*4.294967296e9) +
		double(fileTime.dwLowDateTime);
}

int GetCpuUsage()
{
	_FILETIME idleTime, kernelTime, userTime;
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUserTime);

	return int(100.0 - (dNewIdleTime - dOldIdleTime) /
		(dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime) * 100.0);
}


BOOL CPageProc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	HANDLE hProcess = NULL;
	// 设置标签
	m_Proc.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_Proc.InsertColumn(0, _T("映像名称"), LVCFMT_LEFT, 100);
	m_Proc.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 70);
	m_Proc.InsertColumn(2, _T("CPU"), LVCFMT_LEFT, 60);
	m_Proc.InsertColumn(3, _T("内存"), LVCFMT_LEFT, 50);
	m_Proc.InsertColumn(4, _T("路径"), LVCFMT_LEFT, 400);

	// 获取进程信息
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	//1 创建快照
	HANDLE hSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS, //你要创建的快照类型
		0                   //填进程ID，当创建模块快照或者堆快照的时候，有用
	);

	//2 得到第一个进程信息
	BOOL bSuccess = Process32First(hSnap, &pe);
	
	int iLine = m_Proc.GetItemCount();
	//3 循环遍历所有的进程信息
	if (bSuccess == TRUE)
	{
		do
		{	
			CString id;
			id.Format(L"%d", pe.th32ProcessID); // 获取PID
			m_Proc.InsertItem(iLine, pe.szExeFile, NULL); // 程序名称
			m_Proc.SetItemText(iLine, 1, id);

			// 获取路径
			CString strPath;
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
			
			GetModuleFileNameEx(hProcess, NULL, strPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
			m_Proc.SetItemText(iLine, 4, strPath);

			// 获取内存信息
			CString Info;
// 			MEMORYSTATUS mem;
// 			GlobalMemoryStatus(&mem);
// 			Info.Format(L"%d", mem.dwMemoryLoad);
/*			m_Proc.SetItemText(iLine, 3, Info);*/
			hProcess = GetCurrentProcess();
			PROCESS_MEMORY_COUNTERS pmc;
			GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
			Info.Format(L"%d", (pmc.WorkingSetSize / 1000));
			m_Proc.SetItemText(iLine, 3, Info);

			// 获取cpu占用率
			CString cpu;
			cpu.Format(L"%d%%", GetCpuUsage());
			m_Proc.SetItemText(iLine, 2, cpu);
			
		} while (Process32Next(hSnap, &pe));
	}
	


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPageProc::OnNMRClickListProc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int istat = m_Proc.GetSelectionMark();
	m_strSelectedTopic = m_Proc.GetItemText(istat, 1);

	POINT point;
	HMENU hMenu, hSubMenu;
	GetCursorPos(&point);
	hMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU_TOPICR));
	hSubMenu = GetSubMenu(hMenu, 0);

	SetMenuDefaultItem(hSubMenu, -1, FALSE);
	SetForegroundWindow();

	TrackPopupMenu(hSubMenu, 0, point.x, point.y, 0, m_Proc, NULL);
	*pResult = 0;
}


void CPageProc::OnLvnColumnclickListProc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	// 排序相关代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_SortColum = pNMListView->iSubItem;
	m_bAs != m_bAs; // 升序还是降序

	int count = m_Proc.GetItemCount(); // 行数
	for (int i = 0; i < count; ++i) {
		m_Proc.SetItemData(i, i);
	}
	m_Proc.SortItems(MyListCompar, (LPARAM)&m_Proc);
	*pResult = 0;
}


