// Stub.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Stub.h"

#pragma comment(linker, "/merge:.data=.text")        // 将.data合并到.text  
#pragma comment(linker, "/merge:.rdata=.text")       // 将.rdata合并到.text 
#pragma comment(linker, "/section:.text,RWE")        // 将.text段的属性设置为可读、可写、可执行


void StubStart();

extern "C" _declspec(dllexport) GLOBAL_PARAM g_StubInfo { (DWORD)StubStart };

API g_API;

char* Mymemcpy(char* Dst, char* Src, DWORD dwSize)
{
	char *Origin = Dst;
	for (int i = 0; i < dwSize; i++)
	{
		((char*)Dst)[i] = ((char*)Src)[i];
	}
	return Origin;
}

DWORD Mystrcmp(char* Dst, char const* Src)
{

	DWORD dwRet = 0;

	while (!(dwRet = *Src - *Dst) && *Dst)
		++Src, ++Dst;

	if (dwRet < 0)
		dwRet = -1;
	else if (dwRet > 0)
		dwRet = 1;

	return dwRet;

}

DWORD Mystrlen(const char* Dst)
{
	DWORD dwRet = 0;

	while (Dst[dwRet] != 0)dwRet++;

	return dwRet;
}

void Mymemset(char* Dst, DWORD dwLength)
{
	for (int i = 0; i < dwLength; i++)
	{
		((char*)Dst)[i] = 0;
	}
}

DWORD GetKernel32ModuleHandle()
{
	DWORD dwKernel32Addr = 0;
	_asm
	{
		pushad;
		xor ecx, ecx;
		mov eax, fs:[0x30];
		mov eax, [eax + 0xc];
		mov esi, [eax + 0x1c];
	next_module:
		mov eax, [esi + 0x8];
		mov edi, [esi + 0x20];
		mov esi, [esi];
		cmp[edi + 12 * 2], cx;
		jnz next_module;
		mov dwKernel32Addr, eax;
		popad;
	}
	return dwKernel32Addr;
}

DWORD GetGetProcAddressFunAddr()
{
	DWORD dwKernel32Addr = GetKernel32ModuleHandle();

	// 1. 获取DOS头、NT头
	PIMAGE_DOS_HEADER pDos_Header;
	PIMAGE_NT_HEADERS pNt_Header;
	pDos_Header = (PIMAGE_DOS_HEADER)dwKernel32Addr;
	pNt_Header = (PIMAGE_NT_HEADERS)(dwKernel32Addr + pDos_Header->e_lfanew);

	// 2. 获取导出表项
	PIMAGE_DATA_DIRECTORY   pDataDir;
	PIMAGE_EXPORT_DIRECTORY pExport;
	pDataDir = pNt_Header->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT;
	pExport = (PIMAGE_EXPORT_DIRECTORY)(dwKernel32Addr + pDataDir->VirtualAddress);

	// 3. 获取导出表详细信息
	PDWORD pAddrOfFun = (PDWORD)(pExport->AddressOfFunctions + dwKernel32Addr);
	PDWORD pAddrOfNames = (PDWORD)(pExport->AddressOfNames + dwKernel32Addr);
	PWORD  pAddrOfOrdinals = (PWORD)(pExport->AddressOfNameOrdinals + dwKernel32Addr);

	// 4. 处理以函数名查找函数地址的请求，循环获取ENT中的函数名，并与传入值对比对，如能匹配上
	//    则在EAT中以指定序号作为索引，并取出其地址值。
	DWORD dwFunAddr = 0;
	for (DWORD i = 0; i < pExport->NumberOfNames; i++)
	{
		PCHAR lpFunName = (PCHAR)(pAddrOfNames[i] + dwKernel32Addr);
		if (Mystrcmp(lpFunName, "GetProcAddress") == 0)
		{
			dwFunAddr = pAddrOfFun[pAddrOfOrdinals[i]] + dwKernel32Addr;
			return dwFunAddr;
		}
		if (i == pExport->NumberOfNames - 1)
			return 0;
	}
	return dwFunAddr;
}

void Decode(DWORD dwHash)
{
	PDWORD pdwBegin = (PDWORD)((DWORD)g_StubInfo.dwStart_RVA + g_API.dwImageBase);
	PDWORD pdwEnd = (PDWORD)((DWORD)g_StubInfo.dwEnd_RVA + g_API.dwImageBase);

	DWORD dwOldProtect = 0;
	while (pdwBegin != pdwEnd)
	{
		g_API.VirtualProtect(pdwBegin, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		(*pdwBegin) ^= dwHash;
		g_API.VirtualProtect(pdwBegin, 4, dwOldProtect, &dwOldProtect);
		pdwBegin++;

	}
}

DWORD HashPassWord(char* Src)
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

void InitAPI()
{
	HMODULE hKernel32 = (HMODULE)GetKernel32ModuleHandle();

	g_API.GetProcAddress = (fnGetProcAddress)GetGetProcAddressFunAddr();
	g_API.LoadLibraryExA = (fnLoadLibraryExA)g_API.GetProcAddress((HMODULE)hKernel32, "LoadLibraryExA");
	g_API.LoadLibraryA = (fnLoadLibraryA)g_API.GetProcAddress((HMODULE)hKernel32, "LoadLibraryA");
	g_API.GetModuleHandleW = (fnGetModuleHandleW)g_API.GetProcAddress((HMODULE)hKernel32, "GetModuleHandleW");
	g_API.VirtualProtect = (fnVirtualProtect)g_API.GetProcAddress((HMODULE)hKernel32, "VirtualProtect");
	g_API.VirtualAlloc = (fnVirtualAlloc)g_API.GetProcAddress((HMODULE)hKernel32, "VirtualAlloc");

	HMODULE hUser32 = g_API.LoadLibraryExA("user32.dll", NULL, 0);

	g_API.DefWindowsProcW = (fnDefWindowProcW)g_API.GetProcAddress(hUser32, "DefWindowProcW");
	g_API.RegisterClassExW = (fnRegisterClassExW)g_API.GetProcAddress(hUser32, "RegisterClassExW");
	g_API.CreateWindowExW = (fnCreateWindowExW)g_API.GetProcAddress(hUser32, "CreateWindowExW");
	g_API.ShowWindow = (fnShowWindow)g_API.GetProcAddress(hUser32, "ShowWindow");
	g_API.UpdateWindow = (fnUpdateWindow)g_API.GetProcAddress(hUser32, "UpdateWindow");
	g_API.GetMessageW = (fnGetMessageW)g_API.GetProcAddress(hUser32, "GetMessageW");
	g_API.TranslateMessage = (fnTranslateMessage)g_API.GetProcAddress(hUser32, "TranslateMessage");
	g_API.DispatchMessageW = (fnDispatchMessageW)g_API.GetProcAddress(hUser32, "DispatchMessageW");
	g_API.dwImageBase = (DWORD)g_API.GetModuleHandleW(NULL);
	g_API.DestroyWindow = (fnDestroyWindow)g_API.GetProcAddress(hUser32, "DestroyWindow");
	g_API.PostQuitMessage = (fnPostQuitMessage)g_API.GetProcAddress(hUser32, "PostQuitMessage");
	g_API.ExitProcess = (fnExitProcess)g_API.GetProcAddress((HMODULE)hKernel32, "ExitProcess");

	g_API.GetDlgItemTextA = (fnGetDlgItemTextA)g_API.GetProcAddress(hUser32, "GetDlgItemTextA");
	g_API.MessageBoxW = (fnMessageBoxW)g_API.GetProcAddress(hUser32, "MessageBoxW");
	g_API.SetUnhandledExceptionFilter = (fnSetUnhandledExceptionFilter)g_API.GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");


	g_API.dwFirstChance = 1;

}

void FixOriginRelocation()
{
	if (g_StubInfo.dwReloc_RVA == 0)
		return;

	DWORD dwImageBase = (DWORD)g_API.GetModuleHandleW(NULL);


	PIMAGE_BASE_RELOCATION	pRelocBlock = (PIMAGE_BASE_RELOCATION)((DWORD)dwImageBase + g_StubInfo.dwReloc_RVA);
	while (pRelocBlock->VirtualAddress)
	{
		PTYPEOFFSET pTypeOffset = (PTYPEOFFSET)(pRelocBlock + 1);
		DWORD dwNumber = (pRelocBlock->SizeOfBlock - 8) / sizeof(WORD);

		DWORD dwOldProtect;
		g_API.VirtualProtect((LPVOID)(pRelocBlock->VirtualAddress + dwImageBase), 1, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		for (DWORD i = 0; i < dwNumber; i++)
		{
			if (pTypeOffset[i].Type == 3)
			{
				DWORD dwRVA = pTypeOffset[i].Offset + pRelocBlock->VirtualAddress;
				DWORD nAddressOfReloc = *(DWORD*)(dwImageBase + dwRVA);
				*(DWORD*)(dwImageBase + dwRVA) = nAddressOfReloc - g_StubInfo.dwOrignalImageBase + dwImageBase;
			}
		}
		g_API.VirtualProtect((LPVOID)(pRelocBlock->VirtualAddress + dwImageBase), 1, dwOldProtect, &dwOldProtect);
		pRelocBlock = (PIMAGE_BASE_RELOCATION)((char*)pRelocBlock + pRelocBlock->SizeOfBlock);
	}
}

void FixOriginIAT()
{
	//第一个模块
	IMAGE_IMPORT_DESCRIPTOR* pModule = (IMAGE_IMPORT_DESCRIPTOR*)(g_API.dwImageBase + g_StubInfo.dwIAT_RVA);

	char* szDllName = NULL;
	HMODULE hDll = NULL;

	while (pModule->Name)
	{
		szDllName = (char*)(pModule->Name + g_API.dwImageBase);
		hDll = g_API.LoadLibraryA(szDllName);

		PIMAGE_THUNK_DATA pIAT = (PIMAGE_THUNK_DATA)(pModule->FirstThunk + g_API.dwImageBase);

		PIMAGE_THUNK_DATA pINT =
			(PIMAGE_THUNK_DATA)((pModule->OriginalFirstThunk ? pModule->OriginalFirstThunk : pModule->FirstThunk) + g_API.dwImageBase);

		while (pINT->u1.AddressOfData)//该项有值
		{
			DWORD dwFunAddress = 0;
			//序号导出
			if (IMAGE_SNAP_BY_ORDINAL32(pINT->u1.AddressOfData))
			{
				dwFunAddress = (DWORD)g_API.GetProcAddress(hDll, (char*)(pINT->u1.Ordinal & 0xFFFF));
			}
			else//名字导出
			{
				IMAGE_IMPORT_BY_NAME* pImoprtByName = (IMAGE_IMPORT_BY_NAME*)(g_API.dwImageBase + pINT->u1.AddressOfData);
				dwFunAddress = (DWORD)g_API.GetProcAddress(hDll, (char*)(pImoprtByName->Name));
			}

			DWORD dwOldProtect = 0;

			g_API.VirtualProtect((LPVOID)pIAT, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect);

			pIAT->u1.Function = dwFunAddress;

			g_API.VirtualProtect((LPVOID)pIAT, 1, dwOldProtect, &dwOldProtect);

			pIAT++;
			pINT++;
		}
		pModule++;
	}
}

LRESULT CALLBACK MyWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:break;
	case  WM_COMMAND:
	{
		if ((0x1002 == LOWORD(wParam)) && BN_CLICKED == HIWORD(wParam))
		{

			char szBuf[512];
			Mymemset(szBuf, 512);
			g_API.GetDlgItemTextA(g_API.hParent, 0x1001, szBuf, 512);
			if (Mystrlen(szBuf) == 0)
				break;

			DWORD dwHash = HashPassWord(szBuf);

			Decode(dwHash);

			//FixOriginRelocation();
			//FixOriginIAT();

			g_API.DestroyWindow(g_API.hParent);
			g_API.PostQuitMessage(0);
		}
	}
	break;
	case WM_CLOSE:
	{
		g_API.ExitProcess(0);
	}
	break;
	default:break;
	}
	return g_API.DefWindowsProcW(hWnd, message, wParam, lParam);
}

void PopWindows()
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MyWinProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = (HINSTANCE)g_API.dwImageBase;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = L"password";
	wcex.lpszClassName = L"password";
	wcex.hIconSm = NULL;

	g_API.RegisterClassExW(&wcex);
	g_API.hParent = g_API.CreateWindowExW(NULL, L"password", L"请输入使用密码",
		WS_CAPTION | WS_BORDER | WS_OVERLAPPEDWINDOW, 300, 200, 400, 180,
		NULL, NULL, NULL, NULL);

	if (!g_API.hParent)
	{
		g_API.ExitProcess(0);
		return;
	}

	g_API.ExeWindowsInf[0].hWnd = g_API.CreateWindowExW(WS_EX_TOPMOST, L"edit",
		NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD, 60, 40, 180, 28, g_API.hParent,
		(HMENU)0x1001, NULL, NULL);
	g_API.ExeWindowsInf[1].hWnd = g_API.CreateWindowExW
	(NULL, L"button", L"确认", WS_CHILD | WS_VISIBLE, 100, 80, 80, 30, g_API.hParent,
		(HMENU)0x1002, NULL, NULL);

	g_API.ShowWindow(g_API.hParent, SW_SHOWNORMAL);
	g_API.UpdateWindow(g_API.hParent);

	MSG msg;
	while (g_API.GetMessageW(&msg, NULL, 0, 0) != 0)
	{
		g_API.TranslateMessage(&msg);
		g_API.DispatchMessageW(&msg);
	}

}

LONG WINAPI MyUnhandledExceptionFilter(EXCEPTION_POINTERS *ExceptionInfo)
{
	if (g_API.dwFirstChance)
	{
		g_API.dwFirstChance = 0;
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	g_API.MessageBoxW(NULL, L"您输入的使用密码有误，程序即将退出", L"提示", MB_OK);

	return EXCEPTION_EXECUTE_HANDLER;

}

__declspec(naked) void StubStart()
{
	// 修复
	InitAPI();
	g_StubInfo.dwOriginOEP_RVA += g_API.dwImageBase;
	g_API.SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	PopWindows();
	FixOriginRelocation();
	FixOriginIAT();

	_asm
	{
		jmp g_StubInfo.dwOriginOEP_RVA;
		ret;
	}
}



