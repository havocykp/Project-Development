#pragma once


// CWndList

class CWndList : public CListCtrl
{
	DECLARE_DYNAMIC(CWndList)

public:
	CWndList();
	virtual ~CWndList();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void InsertColumnCustom(DWORD dwCount, ...);
	void InsertItemCustom(DWORD dwCount, ...);
};


