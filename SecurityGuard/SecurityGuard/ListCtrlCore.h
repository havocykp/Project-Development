#pragma once


// CListCtrlCore

class CListCtrlCore : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlCore)

public:
	CListCtrlCore();
	virtual ~CListCtrlCore();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void InsertColumnCustom(DWORD dwCount, ...);
	void InsertItemCustom(DWORD dwCount, ...);
};


