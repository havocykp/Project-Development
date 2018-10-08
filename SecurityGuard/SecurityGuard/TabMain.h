#pragma once


// CTabMain

class CTabMain : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabMain)

public:
	CTabMain();
	virtual ~CTabMain();
public:
	void InsertItemCustom(DWORD dwCount, ...);
	void AddDialogCustom(DWORD dwCount, ...);
	void ShowDialogCustom(INT nIndex);
protected:
	DECLARE_MESSAGE_MAP()
protected:
	CDialogEx*  m_Dialog[10];
	DWORD  m_dwDialogCount;
public:
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnTcnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


