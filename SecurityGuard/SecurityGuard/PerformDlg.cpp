// PerformDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "PerformDlg.h"
#include "afxdialogex.h"
#include <Psapi.h>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")

// CPerformDlg 对话框

IMPLEMENT_DYNAMIC(CPerformDlg, CDialogEx)

CPerformDlg::CPerformDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PERFORMANCE, pParent)
{

}

CPerformDlg::~CPerformDlg()
{
}

void CPerformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_PROGRESS1, m_Progress_MemoryLoad);
	DDX_Control(pDX, IDC_STATIC_TOTAL_PHYS, m_Static_TotalPhys);
	DDX_Control(pDX, IDC_STATIC_AVAIL_PHYS, m_static_AvailPhys);
	DDX_Control(pDX, IDC_STATIC_TOTAL_PAGE_FILE, m_Static_TotalPageFile);
	DDX_Control(pDX, IDC_STATIC_AVAIL_PAGE_FILE, m_Static_AvailPageFile);
	DDX_Control(pDX, IDC_STATIC_TOTAL_VIRTUAL, m_Static_TotalVirtual);
	DDX_Control(pDX, IDC_STATIC_AVAIL_VIRTUAL, m_Static_AvailVirtual);
	//  DDX_Control(pDX, IDC_PROGRESS_USAGE, m_Static_MemoryLoad);
	DDX_Control(pDX, IDC_PROGRESS_USAGE, m_Progress_CpuUsage);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress_MemoryLoad);
	DDX_Control(pDX, IDC_STATIC_MEMORY_LOAD, m_Static_MemoryLoad);
	DDX_Control(pDX, IDC_STATIC_CPU_USAGE, m_Static_CpuUsage);
}


BEGIN_MESSAGE_MAP(CPerformDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_Clear_Mem, &CPerformDlg::OnBnClickedButtonClearMem)
	ON_BN_CLICKED(IDC_BUTTON_SystemInfo, &CPerformDlg::OnBnClickedButtonSysteminfo)
END_MESSAGE_MAP()

void CPerformDlg::UpdatePerformanceData()
{
	//显示内存
	MEMORYSTATUSEX memStatus = { 0 };
	//获取当前内存的状态
	memStatus.dwLength = sizeof(memStatus);

	//MEMORYSTATUS  memStatus; // 这个为什么不能得到正确结果，需要用Ex版函数
	GlobalMemoryStatusEx(&memStatus);
	// MEMORYSTATUS  memStatus;
	//GlobalMemoryStatus(&memStatus);


	//显示数据
	DWORD m = 1024 * 1024;
	CString str;

	// 当前内存的占用率
	m_Progress_MemoryLoad.SetPos(memStatus.dwMemoryLoad);
	str.Format(L"%d%% / 100", memStatus.dwMemoryLoad);
	m_Static_MemoryLoad.SetWindowTextW(str);

	// 总的物理内存大小
	str.Format(L"%d M", memStatus.ullTotalPhys / m);
	m_Static_TotalPhys.SetWindowTextW(str);
	// 可能的物理内存大小 以字节为单位
	str.Format(L"%d M", memStatus.ullAvailPhys / m);
	m_static_AvailPhys.SetWindowTextW(str);
	// 交换文件总大小
	str.Format(L"%d M", memStatus.ullTotalPageFile / m);
	m_Static_TotalPageFile.SetWindowTextW(str);
	// 交换文件中空闲部分大小
	str.Format(L"%d M", memStatus.ullAvailPageFile / m);
	m_Static_AvailPageFile.SetWindowTextW(str);
	// 总的虚拟内存大小
	str.Format(L"%d M", memStatus.ullTotalVirtual / m);
	m_Static_TotalVirtual.SetWindowTextW(str);
	// 可用虚拟内存大小
	str.Format(L"%d M", memStatus.ullAvailVirtual / m);
	m_Static_AvailVirtual.SetWindowTextW(str);


	//CPU占用率
	INT cpuUsage = GetCpuUsage();
	m_Progress_CpuUsage.SetPos(cpuUsage);
	str.Format(L"%d%% / 100", cpuUsage);
	m_Static_CpuUsage.SetWindowTextW(str);
}


INT CPerformDlg::GetCpuUsage()
{
	//		    空闲事件  内核事件   用户事件
	_FILETIME  idleTime, kernelTime, userTime;
	//获取时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	//等待1000毫秒
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);
	//获取新的时间
	_FILETIME  newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	//将各个时间转换
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUserTime);
	//计算出使用率
	return  int(100.0 - (dNewIdleTime - dOldIdleTime) /
		(dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime) * 100.0);
}

double CPerformDlg::FILETIME2Double(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);

}

// CPerformDlg 消息处理程序


BOOL CPerformDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdatePerformanceData();
	SetTimer(100, 10000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}





void CPerformDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 100) {
		UpdatePerformanceData();
	}

	CDialogEx::OnTimer(nIDEvent);
}

//优化内存
void CPerformDlg::OnBnClickedButtonClearMem()
{
	// TODO: 在此添加控件通知处理程序代码
	//1. 获取当前的内存状态
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	//2. 清理内存
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++) {
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}
	//3. 获取清理后的内存状态
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG afterCleanUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;

	
	//self:update
	UpdatePerformanceData();

}

DWORD PASCAL CPerformDlg::GetVersion(void)
{  
	//from:  https://msdn.microsoft.com/en-us/library/aa370663.aspx
	DWORD dwLevel = 102;
	LPWKSTA_INFO_102 pBuf = NULL;
	NET_API_STATUS nStatus;
	LPWSTR pszServerName = NULL;
	//
	// Call the NetWkstaGetInfo function, specifying level 102.
	//
	nStatus = NetWkstaGetInfo(pszServerName,
		dwLevel,
		(LPBYTE *)&pBuf);
	//
	// If the call is successful,
	//  print the workstation data.
	//
	if (nStatus == NERR_Success)
	{
		CString strVersion;
		CString strPlatform = L"Not  NT platform";
		if (pBuf->wki102_platform_id == PLATFORM_ID_NT) {
			strPlatform = L"Windows NT";
		}
		strVersion.Format(L" Platform: %s \n Name:     %s \n Version:  %d.%d   \n Domain:   %s\n Lan Root: %s \n Logged On Users: %d ", 
			strPlatform, pBuf->wki102_computername, pBuf->wki102_ver_major,
			pBuf->wki102_ver_minor, pBuf->wki102_langroup, pBuf->wki102_lanroot, pBuf->wki102_logged_on_users);
		MessageBox(strVersion);
	}
	//
	// Otherwise, indicate the system error.
	//
	else
		MessageBox(L"A system error has occurred");
	//
	// Free the allocated memory.
	//
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);

	return 0;
}


void CPerformDlg::OnBnClickedButtonSysteminfo()
{
	// TODO: 在此添加控件通知处理程序代码
	GetVersion();
}
