// ThreadList.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "ThreadList.h"


// CThreadList

IMPLEMENT_DYNAMIC(CThreadList, CListCtrl)

CThreadList::CThreadList()
{

}

CThreadList::~CThreadList()
{
}


BEGIN_MESSAGE_MAP(CThreadList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CThreadList::OnNMRClick)
	ON_COMMAND(ID_SUSPEND_THREAD, &CThreadList::OnSuspendThread)
	ON_COMMAND(ID_RESUME_THREAD, &CThreadList::OnResumeThread)
	ON_COMMAND(ID_TERMINATE_THREAD, &CThreadList::OnTerminateThread)
END_MESSAGE_MAP()

void CThreadList::InsertColumnCustom(DWORD dwCount, ...)
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


void CThreadList::InsertItemCustom(DWORD dwCount, ...)
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

// CThreadList 消息处理程序




void CThreadList::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
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
		VERIFY(menu.LoadMenu(IDR_MENU2));           //这里是我们在1中定义的MENU的文件名称  
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

		//下面的两行代码主要是为了后面的操作为准备的  
		//获取列表视图控件中第一个被选择项的位置    
		POSITION m_pstion = GetFirstSelectedItemPosition();
		//该函数获取由pos指定的列表项的索引，然后将pos设置为下一个位置的POSITION值  
		INT m_nIndex = GetNextSelectedItem(m_pstion);

		//获取PID
		int nColumn = 1; //tid所在的列
		CString strPID = GetItemText(m_nIndex, nColumn);
		//MessageBox(strPID);
		m_nChoosedTid = _wtoi(strPID);
	}
}


void CThreadList::OnSuspendThread()
{
	// TODO: 在此添加命令处理程序代码
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, false, m_nChoosedTid);
	if (hThread == NULL) {
		MessageBox(L"无法获取线程句柄");
		return;
	}
	DWORD  res = SuspendThread(hThread);
	if (res == -1) {
		MessageBox(L"挂起线程失败");
	}
	else {
		MessageBox(L"挂在线程成功");
	}
}




void CThreadList::OnResumeThread()
{
	// TODO: 在此添加命令处理程序代码
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, false, m_nChoosedTid);
	if (hThread == NULL) {
		MessageBox(L"无法获取线程句柄");
		return;
	}
	DWORD res = ResumeThread(hThread);
	if (res == -1) {
		MessageBox(L"恢复线程失败");
	}
	else {
		MessageBox(L"恢复线程成功");
	}
}


void CThreadList::OnTerminateThread()
{
	// TODO: 在此添加命令处理程序代码
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, false, m_nChoosedTid);
	if (hThread == NULL) {
		MessageBox(L"无法获取线程句柄");
		return;
	}
	if (!TerminateThread(hThread, 0)) {
		MessageBox(L"终止线程失败");
	}
	else {
		MessageBox(L"终止线程成功");
	}
}
