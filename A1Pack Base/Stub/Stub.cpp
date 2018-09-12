// Stub.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Stub.h"

extern __declspec(dllexport) GLOBAL_PARAM g_stcParam = { 0 };
LPGETPROCADDRESS g_funGetProcAddress = nullptr;
LPLOADLIBRARYEX g_funLoadLibraryEx = nullptr;

LPEXITPROCESS g_funExitProcess = nullptr;
LPMESSAGEBOX g_funMessageBox = nullptr;
LPGETMODULEHANDLE g_funGetModuleHandle = nullptr;
LPVIRTUALPROTECT g_funVirtualProtect = nullptr;

// 获取Kernel32.dll的模块基址
DWORD GetKernel32Base()
{
	OutputDebugString(L"GetKernel32Base Function Entry!");
	DWORD dwKernel32Addr = 0;
	__asm
	{
		push eax
		mov eax,dword ptr fs:[0x30] // PEB的地址
		mov eax,[eax+0x0C] // 指向PEB_LDR_DATA结构的指针
		mov eax,[eax+0x1C] // 模块初始化链表的头指针InInitializationOrderModuleList
		mov eax,[eax] // 列表中的第二个条目
		mov eax,[eax+0x08] // 获取到的Kernel32.dll基址
		mov dwKernel32Addr,eax
		pop eax
	}

	return dwKernel32Addr;
}

// 获取GetProcAddress的函数地址
DWORD GetGPAFunAddr()
{
	OutputDebugString(L"GetGPAFunAddr Function Entry!");
	DWORD dwAddrBase = GetKernel32Base();

	// 1. 获取DOS头、NT头
	PIMAGE_DOS_HEADER pDos_Header;
	PIMAGE_NT_HEADERS pNt_Header;
	pDos_Header = (PIMAGE_DOS_HEADER)dwAddrBase;
	pNt_Header = (PIMAGE_NT_HEADERS)(dwAddrBase + pDos_Header->e_lfanew);

	// 2. 获取导出表项
	PIMAGE_DATA_DIRECTORY pDataDir;
	PIMAGE_EXPORT_DIRECTORY pExport;
	pDataDir = pNt_Header->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT;

	// 3. 获取导出表详细信息
	PDWORD pAddrOfFun = (PDWORD)(pExport->AddressOfFunctions + dwAddrBase);
	PDWORD pAddrOfNames = (PDWORD)(pExport->AddressOfNames + dwAddrBase);
	PDWORD pAddrOfOrdinals = (PDWORD)(pExport->AddressOfNameOrdinals + dwAddrBase);

	// 4. 处理以函数名查找函数地址的请求，循环获取ENT中的函数名，并与传入值对比对，如能匹配上
	//	  则在EAT中以指定序号作为索引，并取出其地址值
	DWORD dwFunAddr;
	for (DWORD i = 0; i < pExport->NumberOfNames; i++)
	{
		PCHAR lpFunName = (PCHAR)(pAddrOfNames[i] + dwAddrBase);
		if (!strcmp(lpFunName, "GetProcAddress"))
		{
			dwFunAddr = pAddrOfFun[pAddrOfOrdinals[i]] + dwAddrBase;
			break;
		}
		if (i == pExport->NumberOfNames - 1)
			return 0;
	}

	return dwFunAddr;
}

// 初始化API
bool InitializationAPI()
{
	OutputDebugString(L"InitializationAPI Function Entry!");
	HMODULE hModule;

	// 1. 初始化基础API
	g_funGetProcAddress = (LPGETPROCADDRESS)GetGPAFunAddr();
	g_funLoadLibraryEx = (LPLOADLIBRARYEX)g_funGetProcAddress((HMODULE)GetKernel32Base(), "LoadLibraryExW");

	// 2. 初始化其它API
	hModule = NULL;
	if (!(hModule = g_funLoadLibraryEx(L"kernel32.dll", NULL, NULL))) return false;
	g_funExitProcess = (LPEXITPROCESS)g_funGetProcAddress(hModule, "ExitProcess");
	hModule = NULL;
	if (!(hModule = g_funLoadLibraryEx(L"user32.dll", NULL, NULL))) return false;
	g_funMessageBox = (LPMESSAGEBOX)g_funGetProcAddress(hModule, "MessageBoxW");
	hModule = NULL;
	if (!(hModule = g_funLoadLibraryEx(L"kernel32.dll", NULL, NULL))) return false;
	g_funGetModuleHandle = (LPGETMODULEHANDLE)g_funGetProcAddress(hModule, "GetModuleHandleW");
	hModule = NULL;
	if (!(hModule = g_funLoadLibraryEx(L"kernel32.dll", NULL, NULL))) return false;
	g_funVirtualProtect = (LPVIRTUALPROTECT)g_funGetProcAddress(hModule, "VirtualProtect");

	return true;
}

// 解密函数
void Decrypt()
{
	OutputDebugString(L"Decrypt Function Entry!");
	// 在导出的全局变量中读取需要解密区域的起始于结束VA
	PBYTE lpStart = g_stcParam.lpStartVA;
	PBYTE lpEnd = g_stcParam.lpEndVA;

	// 循环解密
	while (lpStart < lpEnd)
	{
		*lpStart -= 0x88;
		*lpStart ^= 0xA1;
		lpStart++;
	}
}

