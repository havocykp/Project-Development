#pragma once


// CThreadList

class CThreadList : public CListCtrl
{
	DECLARE_DYNAMIC(CThreadList)

public:
	CThreadList();
	virtual ~CThreadList();

public:
	void InsertColumnCustom(DWORD dwCount, ...);
	void InsertItemCustom(DWORD dwCount, ...);
protected:
	DECLARE_MESSAGE_MAP()
public:
	INT m_nChoosedTid;
public:
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSuspendThread();
	afx_msg void OnResumeThread();
	afx_msg void OnTerminateThread();
};


