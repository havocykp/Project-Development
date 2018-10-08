// ListCtrlCore.cpp : 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "ListCtrlCore.h"


// CListCtrlCore

IMPLEMENT_DYNAMIC(CListCtrlCore, CListCtrl)

CListCtrlCore::CListCtrlCore()
{

}

CListCtrlCore::~CListCtrlCore()
{
}


BEGIN_MESSAGE_MAP(CListCtrlCore, CListCtrl)
END_MESSAGE_MAP()
void CListCtrlCore::InsertColumnCustom(DWORD dwCount, ...)
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


void CListCtrlCore::InsertItemCustom(DWORD dwCount, ...)
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


// CListCtrlCore 消息处理程序


