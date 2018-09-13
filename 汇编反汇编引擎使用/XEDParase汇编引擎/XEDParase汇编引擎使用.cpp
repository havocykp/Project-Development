#include <stdio.h>
#include <tchar.h>

 /*
 * XEDParase汇编引擎
 **/


// 导入头文件
#include "XEDParse/XEDParse.h"

#ifdef _WIN64
#pragma comment (lib, "XEDParse/x64/XEDParse_x64.lib")
#else
#pragma comment (lib, "XEDParse/x86/XEDParse_x86.lib")
#endif

void printOpcode(const unsigned char* pOpcode, int nSize)
{
	for (int i = 0; i < nSize; ++i)
	{
		printf("%02X ", pOpcode[i]);
	}
}

int _tmain(int argc, _TCHAR *argv[])
{
	XEDPARSE xed = { 0 };

	do 
	{
		printf("地址 指令：");

		// 接受指令的地址和汇编指令
		scanf_s("%x", &xed.cip);
		gets_s(xed.instr, XEDPARSE_MAXBUFSIZE);

		if (XEDPARSE_OK != XEDParseAssemble(&xed))
		{
			printf("指令错误：%s\n", xed.error);
			continue;
		}

		printf("%08X :", xed.cip);
		printOpcode(xed.dest, xed.dest_size);
		printf("\n");
	} while (*xed.instr);


	return 0;
}