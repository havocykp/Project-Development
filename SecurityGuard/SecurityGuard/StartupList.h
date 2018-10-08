#pragma once


// CStartupList

class CStartupList : public CListCtrl
{
	DECLARE_DYNAMIC(CStartupList)

public:
	CStartupList();
	virtual ~CStartupList();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void InsertColumnCustom(DWORD dwCount, ...);
	void InsertItemCustom(DWORD dwCount, ...);
};


