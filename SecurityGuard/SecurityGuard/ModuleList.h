#pragma once


// CModuleList

class CModuleList : public CListCtrl
{
	DECLARE_DYNAMIC(CModuleList)

public:
	CModuleList();
	virtual ~CModuleList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void InsertColumnCustom(DWORD dwCount, ...);
	void InsertItemCustom(DWORD dwCount, ...);
};


