#pragma once


// CImportThunkList

class CImportThunkList : public CListCtrl
{
	DECLARE_DYNAMIC(CImportThunkList)

public:
	CImportThunkList();
	virtual ~CImportThunkList();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void InsertColumnCustom(DWORD dwCount, ...);
	void InsertItemCustom(DWORD dwCount, ...);
};


