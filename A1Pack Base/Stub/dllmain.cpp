// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "Stub.h"

#pragma comment(linker, "/entry:\"StubEntryPoint\"") // 指定程序入口函数为StubEntryPoint()
#pragma comment(linker, "/merge:.data=.text") // 将.data合并到.text
#pragma comment(linker, "/merge:.rdata=.text") // 将.rdata合并到.text
#pragma comment(linker, "/section:.text,RWE") // 将.text段的属性设置为可读、可写、可执行

// Stub开始函数
void start()
{
	// 1. 初始化所有API
	if (!InitializationAPI()) return;

	// 2. 解密宿主程序
	Decrypt();

	// 3. 询问是否执行解密后的程序
	if (g_stcParam.bShowMessage)
	{
		int nRet = g_funMessageBox(NULL, L"解密完成，是否运行原程序？", L"解密完成", MB_OKCANCEL);
		if (IDCANCEL == nRet) return;
	}

	// 4. 跳转到OEP
	__asm jmp g_stcParam.dwOEP;
}

// Stub部分的入口函数
void __declspec(naked) StubEntryPoint()
{
	__asm sub esp, 0x50; // 抬高栈顶，提高兼容性
	start(); // 执行壳的主体部分
	__asm add esp, 0x50; // 平衡堆栈

	// 主动调用ExitProcess函数退出进程可以解决一些兼容性问题
	if (g_funExitProcess)
	{
		g_funExitProcess(0);
	}

	__asm retn;
}


