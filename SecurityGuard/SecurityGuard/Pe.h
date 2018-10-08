#pragma once
#include  <Windows.h>

class CPe
{
public:
	CPe();
	~CPe();
public:
	BOOL LoadFile(WCHAR* pFileName);
	BOOL IsPeFile();
	DWORD RVA2FOA(DWORD RVA);
	DWORD GetSectionFoaByRva(DWORD RVA);
	IMAGE_SECTION_HEADER* GetSectionHeaderByRva(DWORD RVA);

public:
	WCHAR* m_pFileName;
	char* m_pFileBuffer;
	IMAGE_DOS_HEADER* m_pDosHeader;
	IMAGE_NT_HEADERS* m_pNtHeaders;
	IMAGE_FILE_HEADER* m_pFileHeader;
	IMAGE_OPTIONAL_HEADER* m_pOptionalHeader;
	IMAGE_SECTION_HEADER* m_pSectionHeader;
	IMAGE_DATA_DIRECTORY* m_pDataDir;

};
