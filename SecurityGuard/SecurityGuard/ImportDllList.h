#pragma once


// CImportDllList

class CImportDllList : public CListCtrl
{
	DECLARE_DYNAMIC(CImportDllList)

public:
	CImportDllList();
	virtual ~CImportDllList();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void InsertColumnCustom(DWORD dwCount, ...);
	void InsertItemCustom(DWORD dwCount, ...);
};


