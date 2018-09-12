#pragma once
#include "resource.h"
#include "ProcessingPE.h"
#include <stdlib.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#ifdef A1PACK_BASE_EXPORTS
#define A1PACK_BASE_API __declspec(dllexport)
#else
#define A1PACK_BASE_API __declspec(dllimport)
#endif // A1PACK_BASE_EXPORTS

// 用以保存传递给Stub部分的参数
typedef struct _GLOBAL_PARAM
{
	BOOL bShowMessage; // 是否显示解密信息
	DWORD dwOEP; // 程序入口点
	PBYTE lpStartVA; // 起始虚拟地址 (被异或加密区)
	PBYTE lpEndVA; // 结束虚拟地址 (被异或加密区)
}GLOBAL_PARAM, *PGLOBAL_PARAM;

// 声明一个导出的API，供界面程序调用执行加壳操作
A1PACK_BASE_API bool A1Pack_Base(LPWSTR strPath, bool bShowMsg);

// 加壳时会用到的函数声明
extern void Pretreatment(PBYTE lpCodeStart, PBYTE lpCodeEnd, PE_INFO stcPeInfo); // 预处理函数
extern DWORD Implantation(LPVOID &lpFileData, DWORD dwSize, CProcessingPE* pobjPE, PE_INFO stcPeInfo, GLOBAL_PARAM stcParam); // Stub植入函数
