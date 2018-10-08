#pragma once
#include "ThreadDlg.h"
#include "ModuleDlg.h"
#include "HeapDlg.h"

// CProcessList

class CProcessList : public CListCtrl
{
	DECLARE_DYNAMIC(CProcessList)

public:
	CProcessList();
	virtual ~CProcessList();
public:
	void InsertColumnCustom(DWORD dwCount, ...);
	void InsertItemCustom(DWORD dwCount, ...);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	INT m_nChoosedPid;
	CThreadDlg* m_pThreadDialog;
	CModuleDlg* m_pModuleDialog;
	CHeapDlg* m_pHeapDialog;
public:
	afx_msg void OnShowThread(); // 线程
	afx_msg void OnShowModule(); // 模块
	afx_msg void OnShowHeap(); // 堆
	afx_msg void OnTerminateProcess(); // 结束进程
	afx_msg void OnHook(); // Hook
};


