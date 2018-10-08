// ServiceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "ServiceDlg.h"
#include "afxdialogex.h"
#include <winsvc.h>

// CServiceDlg 对话框

IMPLEMENT_DYNAMIC(CServiceDlg, CDialogEx)

CServiceDlg::CServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_Service, pParent)
{

}

CServiceDlg::~CServiceDlg()
{
}

void CServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Service, m_ServiceList);
}


BEGIN_MESSAGE_MAP(CServiceDlg, CDialogEx)
	//ON_NOTIFY(NM_RCLICK, IDC_LIST_Service, &CServiceDlg::OnRclickListService)
END_MESSAGE_MAP()


// CServiceDlg 消息处理程序


BOOL CServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化表格
	m_ServiceList.InsertColumnCustom(6, 0.12, L"ServiceName", 0.2, L"DisplayName", 0.1, L"Status", 0.1, L"startType", 0.08, L"SerType", 0.1, L"Path");
	UpdateServiceInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CServiceDlg::UpdateServiceInfo()
{
	
	// 1. 打开远程计算机服务控制管理器
	SC_HANDLE hScm = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (NULL == hScm)
	{
		MessageBox(L"无法打开服务");
		return ;
	}

	DWORD dwSize = 0;
	DWORD dwServiceNum = 0;
	EnumServicesStatusEx(hScm, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
		NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);

	LPENUM_SERVICE_STATUS_PROCESS pcBuff = NULL;
	pcBuff = (LPENUM_SERVICE_STATUS_PROCESS)malloc(dwSize);
	memset(pcBuff, 0, dwSize);

	EnumServicesStatusEx(hScm, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
		(LPBYTE)pcBuff, dwSize, &dwSize, &dwServiceNum, NULL, NULL);

	//对应关系
	WCHAR* ServiceStateMap[5] = { L"0", L"已停止", L"2", L"3", L"正在运行" };
	

	//遍历信息  
	for (DWORD i = 0; i<dwServiceNum; i++)
	{
		DWORD dwServiceState = pcBuff[i].ServiceStatusProcess.dwCurrentState;
		DWORD dwServiceType = pcBuff[i].ServiceStatusProcess.dwServiceType;
		CString strServiceType, strServiceState;
		strServiceType.Format(L"%d", dwServiceType);
		if (dwServiceState < 5) {
			strServiceState = ServiceStateMap[dwServiceState];
		}
		else {
			strServiceState.Format(L"%d", dwServiceType);
		}
		//获取服务详情
		SC_HANDLE hService = OpenService(hScm, pcBuff[i].lpServiceName, SERVICE_QUERY_CONFIG);
		//获取缓冲大小
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
		//第二次调用, 获取信息
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		//获取启动类型
		DWORD dwStartType =  pServiceConfig->dwStartType;
		CString strStartType;
		switch (dwStartType) {
		case 2:
			strStartType = L"自动";
			break;
		case 3:
			strStartType = L"手动";
			break;
		case 4:
			strStartType = L"禁用";
			break;
		default:
			strStartType.Format(L"%d", dwStartType);
			break;
		}
		//获取路径信息
		WCHAR* dwPath = pServiceConfig->lpBinaryPathName;
		m_ServiceList.InsertItemCustom(6, pcBuff[i].lpServiceName, pcBuff[i].lpDisplayName, strServiceState,
			strStartType, strServiceType, dwPath);
	}

	getchar();

	CloseServiceHandle(hScm);
	return ;
}


//void CServiceDlg::OnRclickListService(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//	
//}
