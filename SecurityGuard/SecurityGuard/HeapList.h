#pragma once


// CHeapList

class CHeapList : public CListCtrl
{
	DECLARE_DYNAMIC(CHeapList)

public:
	CHeapList();
	virtual ~CHeapList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void InsertColumnCustom(DWORD dwCount, ...);
	void InsertItemCustom(DWORD dwCount, ...);
};


