#pragma once
#include <windows.h>

typedef struct _GLOBAL_PARAM
{
	DWORD dwStubOEP_RVA;	//壳程序的入口点
	DWORD dwOriginOEP_RVA;   //被加壳的程序原始入口点

	DWORD dwStart_RVA;	// 起始虚拟地址（被加密区，载入内存中）
	DWORD dwEnd_RVA;      // 结束虚拟地址（被加密区，载入内存中）

	DWORD dwOrignalImageBase;//被加壳的程序的原始镜像基址

	DWORD dwIAT_RVA; //加壳的导入表地址
	DWORD dwTLS_Size; //加壳的tls数据大小
	DWORD dwTLS_RVA; //加壳的tls虚拟地址 rva
	DWORD dwReloc_RVA; //重定位rva

}GLOBAL_PARAM, *PGLOBAL_PARAM;

typedef struct _PESTC
{
	PIMAGE_DOS_HEADER pDosHead;
	PIMAGE_NT_HEADERS pNtHead;
	PIMAGE_SECTION_HEADER pFirstSectionHead;
	PIMAGE_SECTION_HEADER pLastSectionHead;
	HMODULE hStub;
	PGLOBAL_PARAM pStubInfo;
	DWORD dwFileSize;
}PESTC, *PPESTC;


typedef struct _TYPEOFFSET
{
	WORD Offset : 12; //低12位是相对重定位表的偏移
	WORD Type : 4;
}TYPEOFFSET, *PTYPEOFFSET;


class CShell_Base
{
public:
	CShell_Base();
	~CShell_Base();

public:
	DWORD RVA2Offset(PIMAGE_NT_HEADERS32 pNtHead, DWORD dwAddr);

	bool LoadFile(TCHAR * szFilePath);
	bool SaveFile(TCHAR * szFilePath);

	void PositionPE(char* pPE, PPESTC pPEstc);
	inline DWORD Alignment(DWORD dwRawSize, DWORD dwAlignment);
	void AddSection(char* pNewSection, DWORD dwNewSectionLen, char * szNewSectionName, DWORD dwNewSectionAttribute);
	void LoadStubDll();
	void AddStub();
	void FixStubRelocInDLL();
	void FixStubRelocBlock(char** ppRelocTableBuf, DWORD* lpBufLenth, DWORD* lpSectionCharacter);
	void Encode(DWORD dwHash);
	DWORD HashPassWord(char * Src);
	void ReleaseAll();


protected:
	PESTC m_PEinMem;
};

