#include "stdafx.h"
#include "Pe.h"


CPe::CPe()
{
}


CPe::~CPe()
{
	delete[] m_pFileBuffer;
}


BOOL CPe::LoadFile(WCHAR* pFileName)
{
	m_pFileName = pFileName;
	HANDLE hFile = CreateFile(pFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE) {
		CloseHandle(hFile);
		return FALSE;
	}
	m_pFileBuffer = new char[dwFileSize];
	ZeroMemory(m_pFileBuffer, dwFileSize);
	BOOL  bRead = ReadFile(hFile, m_pFileBuffer, dwFileSize, NULL, NULL);
	if (bRead == FALSE) {
		CloseHandle(hFile);
		return FALSE;
	}
	m_pDosHeader = (IMAGE_DOS_HEADER*)m_pFileBuffer;
	if (m_pDosHeader->e_magic != 0x5a4d) {
		return FALSE;
	}
	m_pNtHeaders = (IMAGE_NT_HEADERS*)(m_pDosHeader->e_lfanew + m_pFileBuffer); // error + fileBase
	m_pFileHeader = (IMAGE_FILE_HEADER*)(&m_pNtHeaders->FileHeader);
	m_pOptionalHeader = (IMAGE_OPTIONAL_HEADER*)(&m_pNtHeaders->OptionalHeader);
	m_pSectionHeader = (IMAGE_SECTION_HEADER*)IMAGE_FIRST_SECTION(m_pNtHeaders);
	m_pDataDir = (IMAGE_DATA_DIRECTORY*)m_pOptionalHeader->DataDirectory;

	CloseHandle(hFile);
	return TRUE;
}
BOOL CPe::IsPeFile()
{
	if (m_pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		return FALSE;
	}
	if (m_pNtHeaders->Signature != IMAGE_NT_SIGNATURE) {
		return FALSE;
	}
	return TRUE;
}
DWORD CPe::RVA2FOA(DWORD RVA)
{
	WORD dwCount = m_pFileHeader->NumberOfSections;
	IMAGE_SECTION_HEADER* pSectionHeader = m_pSectionHeader;
	BOOL bFind = false;
	for (WORD i = 0; i < dwCount; i++) {
		if (RVA >= pSectionHeader->VirtualAddress && RVA < (pSectionHeader->VirtualAddress + pSectionHeader->Misc.VirtualSize)) {
			bFind = true;
			break;
		}
		pSectionHeader++;
	}
	if (!bFind) {
		return 0;
	}
	return RVA - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData;

}

DWORD CPe::GetSectionFoaByRva(DWORD RVA)
{
	WORD dwCount = m_pFileHeader->NumberOfSections;
	IMAGE_SECTION_HEADER* pSectionHeader = m_pSectionHeader;
	BOOL bFind = false;
	for (WORD i = 0; i < dwCount; i++) {
		if (RVA >= pSectionHeader->VirtualAddress && RVA < (pSectionHeader->VirtualAddress + pSectionHeader->Misc.VirtualSize)) {
			bFind = true;
			break;
		}
		pSectionHeader++;
	}
	if (!bFind) {
		return 0;
	}
	return pSectionHeader->PointerToRawData;
}

IMAGE_SECTION_HEADER* CPe::GetSectionHeaderByRva(DWORD RVA)
{
	WORD dwCount = m_pFileHeader->NumberOfSections;
	IMAGE_SECTION_HEADER* pSectionHeader = m_pSectionHeader;
	BOOL bFind = false;
	for (WORD i = 0; i < dwCount; i++) {
		if (RVA >= pSectionHeader->VirtualAddress && RVA < (pSectionHeader->VirtualAddress + pSectionHeader->Misc.VirtualSize)) {
			bFind = true;
			break;
		}
		pSectionHeader++;
	}
	if (!bFind) {
		return 0;
	}
	return pSectionHeader;

}

