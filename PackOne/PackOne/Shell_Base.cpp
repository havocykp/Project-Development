#include "stdafx.h"
#include "Shell_Base.h"


CShell_Base::CShell_Base()
{
	memset(&m_PEinMem, 0, sizeof(PESTC));
}


CShell_Base::~CShell_Base()
{
	if (m_PEinMem.pDosHead)
	{
		char* pFile = (char*)m_PEinMem.pDosHead;
		delete[] pFile;
	}
	if (m_PEinMem.hStub)
	{
		FreeLibrary(m_PEinMem.hStub);
	}
}

/*1.传入NT头指针,2.RVA值*/
/*转换失败返回0*/
DWORD CShell_Base::RVA2Offset(PIMAGE_NT_HEADERS32 pNtHead, DWORD dwAddr)
{
	/*VA=RVA+基址(Image Base)*/
	/*相对偏移=目标的FOA-目标所在段的起始位置FOA*/
	/*相对偏移=目标的RVA-目标所在段的起始位置RVA*/

	WORD wCount = pNtHead->FileHeader.NumberOfSections;
	PIMAGE_SECTION_HEADER pCurSec = IMAGE_FIRST_SECTION(pNtHead);//第一个区段头


	//在头部直接返回
	if (dwAddr < pNtHead->OptionalHeader.SizeOfHeaders)
	{
		return dwAddr;
	}

	//找到RVA落在哪个内存中哪个区段内
	for (WORD i = 0; i < wCount; i++)
	{
		if (dwAddr < (pCurSec->VirtualAddress + pCurSec->SizeOfRawData) &&
			dwAddr >= pCurSec->VirtualAddress)
		{
			//FOA = RVA - 内存中区段的起始位置 + 在文件中区段的起始位置 
			return dwAddr - pCurSec->VirtualAddress + pCurSec->PointerToRawData;
		}
		//下个区段
		pCurSec++;
	}


	return 0;
}

//定位pPE(内存)中的PE结构;
void CShell_Base::PositionPE(char* pPE, PPESTC pPEstc)
{
	pPEstc->pDosHead = (PIMAGE_DOS_HEADER)pPE;
	pPEstc->pNtHead = (PIMAGE_NT_HEADERS)(pPEstc->pDosHead->e_lfanew + pPE);
	pPEstc->pFirstSectionHead = IMAGE_FIRST_SECTION(pPEstc->pNtHead);
	pPEstc->pLastSectionHead =
		pPEstc->pFirstSectionHead + (m_PEinMem.pNtHead->FileHeader.NumberOfSections - 1);
}


/*返回RawSize以Alignment为基数进行对齐的大小*/
inline DWORD CShell_Base::Alignment(DWORD dwRawSize, DWORD dwAlignment)
{
	if (dwRawSize%dwAlignment == 0)
	{
		return dwRawSize;
	}
	else
	{
		return (dwRawSize / dwAlignment + 1)*dwAlignment;
	}
}

/*在m_NewPE的区段基础上加一个区段*/
/*1.区段的数据 2.区段数据的大小 3.区段名字 4.区段的属性*/
void CShell_Base::AddSection(char* pNewSection, DWORD dwNewSectionLen,
	char * szNewSectionName, DWORD dwNewSectionAttribute)
{

	DWORD dwOriginFileSize = m_PEinMem.dwFileSize;
	//1 申请一块新的空间，用来存放添加区段后的PE文件
	m_PEinMem.dwFileSize = m_PEinMem.dwFileSize +
		Alignment(dwNewSectionLen, m_PEinMem.pNtHead->OptionalHeader.FileAlignment);

	char* pTmp = new char[m_PEinMem.dwFileSize];

	memset(pTmp, 0, m_PEinMem.dwFileSize);

	//将原PE文件拷贝到新内存中.
	memcpy(pTmp, (char*)m_PEinMem.pDosHead, dwOriginFileSize);


	//释放原来占用的资源
	char* pFileBuf = (char*)m_PEinMem.pDosHead;
	delete[] pFileBuf;

	//定位新内存中的PE结构;
	PositionPE(pTmp, &m_PEinMem);


	//2 开始添加新区段
	//2.1 修改头部信息，包括：区段表，PE头中的区段数量，扩展头中的SizeOfImage
	//2.2 找到区段表的最后一项
	PIMAGE_SECTION_HEADER pOriginLastSection =
		m_PEinMem.pFirstSectionHead + (m_PEinMem.pNtHead->FileHeader.NumberOfSections - 1);

	PIMAGE_SECTION_HEADER pNewLastSection =
		m_PEinMem.pFirstSectionHead + m_PEinMem.pNtHead->FileHeader.NumberOfSections;

	//3 开始填充区段表信息
	strcpy_s((char *)pNewLastSection->Name, 8, szNewSectionName);//区段名

	pNewLastSection->Characteristics = dwNewSectionAttribute;//区段属性

	//区段的文件偏移
	pNewLastSection->PointerToRawData = pOriginLastSection->PointerToRawData +
		pOriginLastSection->SizeOfRawData;

	//区段在文件中的大小
	pNewLastSection->SizeOfRawData =
		Alignment(dwNewSectionLen, m_PEinMem.pNtHead->OptionalHeader.FileAlignment);

	//区段起始位置的RVA
	pNewLastSection->VirtualAddress = pOriginLastSection->VirtualAddress +
		Alignment(pOriginLastSection->Misc.VirtualSize, m_PEinMem.pNtHead->OptionalHeader.SectionAlignment);

	pNewLastSection->Misc.VirtualSize = dwNewSectionLen;//未对齐的数据大小

	m_PEinMem.pNtHead->FileHeader.NumberOfSections++;//增加区段数目

	//加载到内存中所需的内存大小（经过块对齐）
	m_PEinMem.pNtHead->OptionalHeader.SizeOfImage = m_PEinMem.pNtHead->OptionalHeader.SizeOfImage +
		Alignment(dwNewSectionLen, m_PEinMem.pNtHead->OptionalHeader.SectionAlignment);

	//4.在PE文件的后面添加一个新区段
	memcpy((char*)((DWORD)m_PEinMem.pDosHead + pNewLastSection->PointerToRawData), pNewSection, dwNewSectionLen);

	//重新定位新内存中的PE结构;
	PositionPE(pTmp, &m_PEinMem);
}

void CShell_Base::LoadStubDll()
{
	m_PEinMem.hStub = LoadLibrary(L"Stub.dll");
	m_PEinMem.pStubInfo = (PGLOBAL_PARAM)GetProcAddress(m_PEinMem.hStub, "g_StubInfo");
}

void CShell_Base::AddStub()
{
	//1.得到原程序的OEP，设置到pStubInfo中
	m_PEinMem.pStubInfo->dwOriginOEP_RVA = m_PEinMem.pNtHead->OptionalHeader.AddressOfEntryPoint;

	//2.定位hStub的PE结构
	PIMAGE_DOS_HEADER pDosHead = (PIMAGE_DOS_HEADER)m_PEinMem.hStub;
	PIMAGE_NT_HEADERS pNtHead = (PIMAGE_NT_HEADERS)(pDosHead->e_lfanew + (LONG)m_PEinMem.hStub);
	PIMAGE_SECTION_HEADER pFirstSection = IMAGE_FIRST_SECTION(pNtHead);

	DWORD dwOEPinDll_RVA = pNtHead->OptionalHeader.AddressOfEntryPoint;


	//传入数据到Stub
	m_PEinMem.pStubInfo->dwIAT_RVA =
		m_PEinMem.pNtHead->OptionalHeader.DataDirectory[1].VirtualAddress;

	//传参给Stub
	m_PEinMem.pStubInfo->dwOrignalImageBase = m_PEinMem.pNtHead->OptionalHeader.ImageBase;

	//将原重定位表RVA同步到Stub内
	m_PEinMem.pStubInfo->dwReloc_RVA =
		m_PEinMem.pNtHead->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;

	//加密起初区段
	m_PEinMem.pStubInfo->dwStart_RVA = m_PEinMem.pFirstSectionHead->VirtualAddress;

	DWORD dwRawSize = m_PEinMem.pFirstSectionHead->Misc.VirtualSize;

	m_PEinMem.pStubInfo->dwEnd_RVA =
		m_PEinMem.pFirstSectionHead->VirtualAddress + dwRawSize - (dwRawSize % 4);

	m_PEinMem.pStubInfo->dwEnd_RVA -= 4;

	//取Stub运行代码的起点
	DWORD dwStubOEPRVA = m_PEinMem.pStubInfo->dwStubOEP_RVA - (DWORD)m_PEinMem.hStub;

	DWORD dwStubOEPOffsetInSection = dwStubOEPRVA - pNtHead->OptionalHeader.BaseOfCode;

	FixStubRelocInDLL();

	AddSection((char*)(pFirstSection->VirtualAddress + (DWORD)m_PEinMem.hStub),
		pFirstSection->Misc.VirtualSize, ".Stub", pFirstSection->Characteristics);

	//生成基于exe的新OEP
	DWORD dwNewOEP = dwStubOEPOffsetInSection + m_PEinMem.pLastSectionHead->VirtualAddress;

	char* pRelocBuf = NULL;
	DWORD dwBufSize = 0;
	DWORD dwRelocSectionCharacter = 0;
	FixStubRelocBlock(&pRelocBuf, &dwBufSize, &dwRelocSectionCharacter);
	// 在目标程序构建一个重定位表
	AddSection(pRelocBuf, dwBufSize, ".Reloc", dwRelocSectionCharacter);

	//释放存储.Rreloc的资源
	delete[] pRelocBuf;
	pRelocBuf = NULL;
	//将重定位表指向Stub的重定位表
	m_PEinMem.pNtHead->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress
		= m_PEinMem.pLastSectionHead->VirtualAddress;

	m_PEinMem.pNtHead->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size = dwBufSize;


	//清除导入表信息
	m_PEinMem.pNtHead->OptionalHeader.DataDirectory[1].Size = 0;

	m_PEinMem.pNtHead->OptionalHeader.DataDirectory[1].VirtualAddress = 0;


	//清除IAT
	m_PEinMem.pNtHead->OptionalHeader.DataDirectory[12].Size = 0;

	m_PEinMem.pNtHead->OptionalHeader.DataDirectory[12].VirtualAddress = 0;


	//设置目标程序的OEP到新区段中
	m_PEinMem.pNtHead->OptionalHeader.AddressOfEntryPoint = dwNewOEP;

	//去掉随机基址
	//m_PEinMem.pNtHead->OptionalHeader.DllCharacteristics &= ~IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE;
}

void CShell_Base::FixStubRelocInDLL()
{
	//定位Dll的PE信息
	PIMAGE_DOS_HEADER pDosHead = (PIMAGE_DOS_HEADER)m_PEinMem.hStub;
	PIMAGE_NT_HEADERS pNtHead = (PIMAGE_NT_HEADERS)(pDosHead->e_lfanew + (LONG)m_PEinMem.hStub);
	PIMAGE_SECTION_HEADER pFirstSection = IMAGE_FIRST_SECTION(pNtHead);

	//修复重定位信息
	PIMAGE_DATA_DIRECTORY pRelocDir = pNtHead->OptionalHeader.DataDirectory + 5;
	PIMAGE_BASE_RELOCATION pRelocBlock = (PIMAGE_BASE_RELOCATION)(pRelocDir->VirtualAddress + (DWORD)m_PEinMem.hStub);

	DWORD dwNewSectionRVA = m_PEinMem.pLastSectionHead->VirtualAddress +
		Alignment(m_PEinMem.pLastSectionHead->SizeOfRawData, m_PEinMem.pNtHead->OptionalHeader.SectionAlignment);

	//现区段基址-原区段基址=区段差值
	DWORD dwDifference = (dwNewSectionRVA - pNtHead->OptionalHeader.BaseOfCode);



	while (pRelocBlock->SizeOfBlock != 0 && pRelocBlock->VirtualAddress >= pFirstSection->VirtualAddress
		&& pRelocBlock->VirtualAddress < (pFirstSection->VirtualAddress + pFirstSection->Misc.VirtualSize))
	{
		DWORD dwCount = (pRelocBlock->SizeOfBlock - 8) / sizeof(WORD);
		PTYPEOFFSET pTypeOffset = (PTYPEOFFSET)(pRelocBlock + 1);
		for (DWORD i = 0; i < dwCount; i++)
		{
			if (pTypeOffset[i].Type == 3)
			{
				DWORD dwRVA = pTypeOffset[i].Offset + pRelocBlock->VirtualAddress;


				// 修复重定位信息   公式：需要修复的地址-原映像基址-原区段基址+现区段基址+现映像基址
				//FAR地址（LordPE中这样标注）
				DWORD AddrOfNeedReloc = *(DWORD*)((DWORD)pDosHead + dwRVA);

				DWORD dwRelocItemRVA = AddrOfNeedReloc - pNtHead->OptionalHeader.ImageBase;

				DWORD dwRelocItemVAinNewPE = dwRelocItemRVA + m_PEinMem.pNtHead->OptionalHeader.ImageBase;

				*(DWORD*)((DWORD)pDosHead + dwRVA) = (dwRelocItemVAinNewPE + dwDifference);

			}

		}
		pRelocBlock = (PIMAGE_BASE_RELOCATION)((char *)pRelocBlock + pRelocBlock->SizeOfBlock);
	}
}


/*输出修复重定位后的重定位块Buf,还有这个Buf的大小、区段的属性*/
void CShell_Base::FixStubRelocBlock(char** ppRelocTableBuf, DWORD* lpBufLenth, DWORD* lpSectionCharacter)
{
	//定位Dll的PE信息
	PIMAGE_DOS_HEADER pDosHead = (PIMAGE_DOS_HEADER)m_PEinMem.hStub;
	PIMAGE_NT_HEADERS pNtHead = (PIMAGE_NT_HEADERS)(pDosHead->e_lfanew + (LONG)m_PEinMem.hStub);
	PIMAGE_SECTION_HEADER pFirstSection = IMAGE_FIRST_SECTION(pNtHead);

	//修复重定位信息
	PIMAGE_DATA_DIRECTORY pRelocDir = pNtHead->OptionalHeader.DataDirectory + 5;
	PIMAGE_BASE_RELOCATION pRelocBlock = (PIMAGE_BASE_RELOCATION)(pRelocDir->VirtualAddress + (DWORD)m_PEinMem.hStub);
	DWORD dwTotalBlockSize = 0;

	PIMAGE_SECTION_HEADER pLastSection = pFirstSection + (pNtHead->FileHeader.NumberOfSections - 1);

	//取DLL最后一个段的属性
	*lpSectionCharacter = pLastSection->Characteristics;

	//只修复dll中指向第一个区段的重定位块
	while (pRelocBlock->SizeOfBlock != 0 && pRelocBlock->VirtualAddress >= pFirstSection->VirtualAddress
		&& pRelocBlock->VirtualAddress < (pFirstSection->VirtualAddress + pFirstSection->Misc.VirtualSize))
	{

		//统计要修复要复制多少重定位块
		dwTotalBlockSize += pRelocBlock->SizeOfBlock;

		DWORD dwOldProtect;

		VirtualProtect(pRelocBlock, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect);

		//将重定位块RVA修复成适用于EXE的RVA（重定位块在最后的区段，Stub代码在倒数第二个区段）

		DWORD* pNeed2Fix = (DWORD*)pRelocBlock;

		*pNeed2Fix = (pRelocBlock->VirtualAddress - pFirstSection->VirtualAddress + m_PEinMem.pLastSectionHead->VirtualAddress);

		VirtualProtect(pRelocBlock, 1, dwOldProtect, &dwOldProtect);

		//下一个重定位块
		pRelocBlock = (PIMAGE_BASE_RELOCATION)((char*)pRelocBlock + pRelocBlock->SizeOfBlock);
	}

	*ppRelocTableBuf = new char[dwTotalBlockSize + 8];

	memset(*ppRelocTableBuf, 0, dwTotalBlockSize + 8);

	memcpy(*ppRelocTableBuf, (char*)(pRelocDir->VirtualAddress + (DWORD)m_PEinMem.hStub), dwTotalBlockSize);

	*lpBufLenth = dwTotalBlockSize + 8;


}

void CShell_Base::Encode(DWORD dwHash)
{
	DWORD dwImageBase = (DWORD)m_PEinMem.pDosHead;
	DWORD dwBegin = RVA2Offset(m_PEinMem.pNtHead, m_PEinMem.pStubInfo->dwStart_RVA) + dwImageBase;
	DWORD dwEnd = RVA2Offset(m_PEinMem.pNtHead, m_PEinMem.pStubInfo->dwEnd_RVA) + dwImageBase;

	PDWORD pdwBegin = (PDWORD)dwBegin;
	PDWORD pdwEnd = (PDWORD)dwEnd;

	while (pdwBegin != pdwEnd)
	{
		(*pdwBegin) ^= dwHash;
		pdwBegin++;
	}

}

DWORD CShell_Base::HashPassWord(char * Src)
{
	DWORD dwRet = 0;

	while (*Src)
	{
		dwRet = ((dwRet << 25) | (dwRet >> 7));
		dwRet = dwRet + *Src;
		Src++;
	}

	return dwRet;
}


void CShell_Base::ReleaseAll()
{
	if (m_PEinMem.pDosHead)
	{
		char* pBuf = (char*)m_PEinMem.pDosHead;
		delete[]pBuf;
		m_PEinMem.pDosHead = NULL;
		m_PEinMem.pNtHead = NULL;
		m_PEinMem.dwFileSize = NULL;
		m_PEinMem.pFirstSectionHead = NULL;
		m_PEinMem.pLastSectionHead = NULL;
	}

	if (m_PEinMem.hStub)
	{
		FreeLibrary(m_PEinMem.hStub);
		m_PEinMem.hStub = NULL;
		m_PEinMem.pStubInfo = NULL;
	}
}

/*传入文件路径，将文件读入内存中*/
/*正常且为Win32PE则返回true且重新设置m_OriginPE*/
bool CShell_Base::LoadFile(TCHAR *szFilePath)
{
	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	char* pFileBuf = new char[dwFileSize];
	DWORD dwRead = 0;

	//读取文件出错，还原全部操作
	if (!ReadFile(hFile, pFileBuf, dwFileSize, &dwRead, NULL) || dwFileSize != dwRead)
	{
		delete[] pFileBuf;
		pFileBuf = NULL;
		CloseHandle(hFile);
		return false;
	}

	PIMAGE_DOS_HEADER pDosHead = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS32 pNtHead =
		(PIMAGE_NT_HEADERS32)((LONG)pFileBuf + pDosHead->e_lfanew);


	if (pDosHead->e_magic != IMAGE_DOS_SIGNATURE ||
		pNtHead->OptionalHeader.Magic != 0x10B ||
		pNtHead->Signature != IMAGE_NT_SIGNATURE)	//DOS头标记不是5A4D或NT头标记不是4550,PE不是Win32的
	{
		delete[] pFileBuf;
		pFileBuf = NULL;
		CloseHandle(hFile);
		return false;
	}

	m_PEinMem.dwFileSize = dwFileSize;
	PositionPE(pFileBuf, &m_PEinMem);

	CloseHandle(hFile);
	return true;
}

/*传入文件路径，将内存写入文件中，正常则返回true*/
bool CShell_Base::SaveFile(TCHAR * szFilePath)
{
	HANDLE hFile = CreateFile(szFilePath, GENERIC_WRITE, NULL,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	DWORD dwWrite = 0;

	//读取文件出错，关闭句柄
	if (!WriteFile(hFile, m_PEinMem.pDosHead, m_PEinMem.dwFileSize, &dwWrite, NULL) ||
		m_PEinMem.dwFileSize != dwWrite)
	{
		CloseHandle(hFile);
		return false;
	}

	CloseHandle(hFile);
	return true;
}

