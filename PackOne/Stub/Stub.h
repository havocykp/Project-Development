#pragma once
#include <windows.h>

typedef struct _GLOBAL_PARAM
{
	DWORD dwStubOEP_RVA;	//壳程序的入口点
	DWORD dwOriginOEP_RVA;   //被加壳的程序原始入口点

	DWORD dwStart_RVA;	// 起始虚拟地址（被加密区，载入内存中）
	DWORD dwEnd_RVA;      // 结束虚拟地址（被加密区，载入内存中）

	DWORD dwOrignalImageBase;//被加壳的程序的原始镜像基址

	DWORD dwIAT_RVA;//加壳的导入表地址
					//加壳的tls数据大小
	DWORD dwTLS_Size;
	//加壳的tls虚拟地址 rva
	DWORD dwTLS_RVA;
	//重定位rva
	DWORD dwReloc_RVA;

}GLOBAL_PARAM, *PGLOBAL_PARAM;

typedef struct _TYPEOFFSET
{
	WORD Offset : 12;		//偏移值
	WORD Type : 4;			//重定位属性(方式)
}TYPEOFFSET, *PTYPEOFFSET;


//定义一些函数指针

typedef HMODULE(WINAPI*fnGetModuleHandleW)(_In_opt_ LPCWSTR lpModuleName);

typedef HMODULE(WINAPI*fnLoadLibraryExA)(_In_ LPCSTR lpLibFileName, HANDLE file, DWORD mode);

typedef HMODULE(WINAPI*fnLoadLibraryA)(_In_ LPCSTR lpLibFileName);

typedef  DWORD(WINAPI *fnGetProcAddress)(_In_ HMODULE hModule, _In_ LPCSTR lpProcName);

typedef BOOL(WINAPI *fnVirtualProtect)(LPVOID, SIZE_T, DWORD, PDWORD); // VirtualProtect

typedef LPVOID(WINAPI*fnVirtualAlloc)(_In_opt_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD flAllocationType, _In_ DWORD flProtect);

typedef HWND(WINAPI *fnCreateWindowExW)(_In_ DWORD dwExStyle, _In_opt_ LPCWSTR lpClassName, _In_opt_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam);

typedef WORD(WINAPI* fnRegisterClassExW)(_In_ CONST WNDCLASSEXW *lpWndClass);

typedef BOOL(WINAPI* fnShowWindow)(_In_ HWND hWnd, _In_ int nCmdShow);

typedef BOOL(WINAPI* fnUpdateWindow)(_In_ HWND hWnd);

typedef BOOL(WINAPI* fnGetMessageW)(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax);

typedef BOOL(WINAPI* fnTranslateMessage)(_In_ CONST MSG *lpMsg);

typedef LRESULT(WINAPI* fnDispatchMessageW)(_In_ CONST MSG *lpMsg);

typedef  LRESULT(WINAPI* fnDefWindowProcW)(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

typedef VOID(WINAPI *fnPostQuitMessage)(_In_ int nExitCode);

typedef VOID(WINAPI* fnExitProcess)(_In_ UINT uExitCode);

typedef BOOL(WINAPI* fnDestroyWindow)(_In_ HWND hWnd);

typedef UINT(WINAPI* fnGetDlgItemTextA)(_In_ HWND hDlg, _In_ int nIDDlgItem, _Out_writes_(cchMax) LPSTR lpString, _In_ int cchMax);

typedef int (WINAPI* fnMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType);

typedef LPTOP_LEVEL_EXCEPTION_FILTER(WINAPI* fnSetUnhandledExceptionFilter)(_In_opt_ LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter);



typedef struct _SHELLWINDOWSINFO
{
	HWND hWnd;
	HMENU hID;
}SHELLWINDOWSINFO, *PSHELLWINDOWSINFO;

typedef struct _API
{
	fnGetProcAddress GetProcAddress;
	fnLoadLibraryExA LoadLibraryExA;
	fnLoadLibraryA LoadLibraryA;
	fnGetModuleHandleW GetModuleHandleW;
	fnVirtualProtect VirtualProtect;
	fnVirtualAlloc VirtualAlloc;

	fnDefWindowProcW DefWindowsProcW;
	fnRegisterClassExW RegisterClassExW;
	fnCreateWindowExW CreateWindowExW;
	fnShowWindow ShowWindow;
	fnUpdateWindow UpdateWindow;
	fnGetMessageW GetMessageW;
	fnTranslateMessage TranslateMessage;
	fnDispatchMessageW DispatchMessageW;
	fnPostQuitMessage PostQuitMessage;
	fnDestroyWindow DestroyWindow;
	fnExitProcess ExitProcess;

	fnGetDlgItemTextA GetDlgItemTextA;
	fnMessageBoxW MessageBoxW;
	fnSetUnhandledExceptionFilter SetUnhandledExceptionFilter;

	DWORD dwFirstChance;

	DWORD dwImageBase;
	HWND hParent;
	SHELLWINDOWSINFO ExeWindowsInf[2];

}API, *PAPI;

