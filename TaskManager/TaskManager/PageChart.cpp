// PageChart.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "PageChart.h"
#include "afxdialogex.h"


// CPageChart 对话框

IMPLEMENT_DYNAMIC(CPageChart, CDialogEx)

CPageChart::CPageChart(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PAGE_CHART, pParent)
{

}

CPageChart::~CPageChart()
{
}

void CPageChart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Chart, m_Chart);
}


BEGIN_MESSAGE_MAP(CPageChart, CDialogEx)
END_MESSAGE_MAP()


// CPageChart 消息处理程序


BOOL CPageChart::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
