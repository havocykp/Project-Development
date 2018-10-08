#pragma once


// CExportDirList

class CExportDirList : public CListCtrl
{
	DECLARE_DYNAMIC(CExportDirList)

public:
	CExportDirList();
	virtual ~CExportDirList();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void InsertColumnCustom(DWORD dwCount, ...);
	void InsertItemCustom(DWORD dwCount, ...);
};


