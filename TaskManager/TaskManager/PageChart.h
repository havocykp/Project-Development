#pragma once
#include "afxcmn.h"


// CPageChart 对话框

class CPageChart : public CDialogEx
{
	DECLARE_DYNAMIC(CPageChart)

public:
	CPageChart(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageChart();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_CHART };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Chart;
	virtual BOOL OnInitDialog();
};
