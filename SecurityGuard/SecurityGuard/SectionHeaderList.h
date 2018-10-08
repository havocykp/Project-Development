#pragma once


// CSectionHeaderList

class CSectionHeaderList : public CListCtrl
{
	DECLARE_DYNAMIC(CSectionHeaderList)

public:
	CSectionHeaderList();
	virtual ~CSectionHeaderList();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void InsertColumnCustom(DWORD dwCount, ...);
	void InsertItemCustom(DWORD dwCount, ...);
};


