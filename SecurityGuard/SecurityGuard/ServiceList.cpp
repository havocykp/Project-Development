// ServiceList.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "ServiceList.h"

#include <winsvc.h>

// CServiceList

IMPLEMENT_DYNAMIC(CServiceList, CListCtrl)

CServiceList::CServiceList()
{

}

CServiceList::~CServiceList()
{
}


BEGIN_MESSAGE_MAP(CServiceList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CServiceList::OnNMRClick)
	ON_COMMAND(ID_StartService, &CServiceList::OnStartService)
END_MESSAGE_MAP()

void CServiceList::InsertColumnCustom(DWORD dwCount, ...)
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

		if (nWidth < 5) {
			MessageBox(L"Warning: 线程列表的宽度太小了\n");
		}

		WCHAR* wStr = va_arg(list, WCHAR*);

		InsertColumn(i, wStr, 0, nWidth);
		nUsedWidth += nWidth;
	}
	va_end(list);

	return;
}


void CServiceList::InsertItemCustom(DWORD dwCount, ...)
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

// CServiceList 消息处理程序




void CServiceList::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
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
		VERIFY(menu.LoadMenu(IDR_MENU3));           //这里是我们在1中定义的MENU的文件名称  
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
		CString strServiceName = GetItemText(m_nIndex, nColumn);
		m_strClickedServiceName = strServiceName;
	}
}



//实际为切换服务状态
void CServiceList::OnStartService()
{
	// TODO: 在此添加命令处理程序代码
	// 打开服务管理对象  
	SC_HANDLE hSC = OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);// GENERIC_EXECUTE
	if (hSC == NULL)
	{
		MessageBox(L"open SCManager(GENERIC_EXECUTE) error");
		return;
	}
	// 打开www服务。  
	SC_HANDLE hSvc = ::OpenService(hSC, m_strClickedServiceName,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		MessageBox(L"OpenService error");
		::CloseServiceHandle(hSC);
		return;
	}
	// 获得服务的状态  
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		MessageBox(L"Get Service state error");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
	}
	//如果处于停止状态则启动服务，否则停止服务。  
	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		// 停止服务  
		if (::ControlService(hSvc,
			SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			MessageBox(L"stop service error");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// 等待服务停止  
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				MessageBox(L"stop success");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return;
			}
		}
	}
	else if (status.dwCurrentState == SERVICE_STOPPED)
	{
		// 启动服务  
		if (::StartService(hSvc, NULL, NULL) == FALSE)
		{
			MessageBox(L"start service error");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// 等待服务启动  
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				MessageBox(L"start success");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return;
			}
		}
	}
	MessageBox(L"start error");
	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(hSC);
	return;
}


