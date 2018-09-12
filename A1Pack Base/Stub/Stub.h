#pragma once
#include <windows.h>

// 声明一个导出的全局变量，用以保存传递给Stub部分的参数
typedef struct _GLOBAL_PARAM
{
	BOOL bShowMessage; // 是否显示解密信息
	DWORD dwOEP; // 程序入口点
	PBYTE lpStartVA; // 起始虚拟地址（被异或加密区）
	PBYTE lpEndVA; // 结束虚拟地址（被异或加密区）
}GLOBAL_PARAM, *PGLOBAL_PARAM;
extern "C" __declspec(dllexport) GLOBAL_PARAM g_stcParam;

// 基础API定义声明
typedef DWORD(WINAPI *LPGETPROCADDRESS)(HMODULE, LPCSTR); // GetProcAddress
typedef HMODULE(WINAPI *LPLOADLIBRARYEX)(LPCTSTR, HANDLE, DWORD); // LoadLibraryEx
extern LPGETPROCADDRESS g_funGetProcAddress;
extern LPLOADLIBRARYEX g_funLoadLibraryEx;

// 其它API定义声明
typedef VOID(WINAPI *LPEXITPROCESS)(UINT); // ExitProcess
typedef int (WINAPI *LPMESSAGEBOX)(HWND, LPCTSTR, LPCTSTR, UINT); // MessageBOx
typedef HMODULE(WINAPI *LPGETMODULEHANDLE)(LPCWSTR); // GetModuleHandle
typedef BOOL(WINAPI *LPVIRTUALPROTECT)(LPVOID, SIZE_T, DWORD, PDWORD); // VirtualProtect
extern LPEXITPROCESS g_funExitProcess;
extern LPMESSAGEBOX g_funMessageBox;
extern LPGETMODULEHANDLE g_funGetModuleHandle;
extern LPVIRTUALPROTECT g_funVirtualProtect;

// 声明功能函数
extern DWORD GetKernel32Base(); // 获取Kernel32.dll的模块基址
extern DWORD GetGPAFunAddr(); // 获取GetProcAddress的函数地址
extern bool InitializationAPI(); // 初始化各个API
extern void Decrypt(); // 解密函数