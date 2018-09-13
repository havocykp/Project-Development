#include <stdio.h>
#include <tchar.h>
#include <inttypes.h>
#include "include/capstone.h"

 /*
 * Capstone反汇编引擎
 **/

// 包含头文件
#ifdef _WIN64
#pragma comment(lib, "capstone_x64.lib")
#else
#pragma comment(lib, "capstone_x86.lib")
#endif // _WIN64


int _tmain(int argc, _TCHAR *argv[])
{
	csh handle; // 反汇编引擎句柄
	cs_err err; // 错误信息
	cs_insn *pInsn; // 保存反汇编得到的指令的缓冲区首地址
	unsigned int count = 0; // 保存得到的反汇编的指令条数

	// 在编译capstone库的时候, 如果选择了用户自定义堆空间管理组(没有定义`CAPSTONE_USE_SYS_DYN_MEM`宏)
	// 则需要调用cs_option函数来设定堆空间管理组函数
	// 然后用结构体来配置回调函数.

	// 定义结构体，配置堆空间的回调函数
	cs_opt_mem memop;
	memop.calloc = calloc;
	memop.free = free;
	memop.malloc = malloc;
	memop.realloc = realloc;
	memop.vsnprintf = (cs_vsnprintf_t)vsprintf_s;
	// 注册堆空间管理组函数
	cs_option(0, CS_OPT_MEM, (size_t)&memop);

	// 初始化反汇编句柄，(x86_64架构，32模式，句柄)
	err = cs_open(CS_ARCH_X86,/*x86指令集*/
				  CS_MODE_32,/*使用32位模式解析opcode*/
				  &handle/*输出的反汇编句柄*/
			    );

	if (err != CS_ERR_OK)
	{
		printf("初始化反汇编器句柄失败\n");
		return 0;
	}

	unsigned char Opcode[] = 
	{

		0x6B, 0x4C, 0x73, 0x45, 0x00, 0x00, 0x00, 0x01, 0x90, 0x5C, 0x4D, 0x77, 0x00, 0x00, 0x00, 0x00,
		0x60, 0x17, 0x3C, 0x77, 0x60, 0x77, 0x42, 0x77, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x84, 0x17, 0x3C, 0x77, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x57, 0x14, 0x01, 0xE2, 0x46, 0x15, 0xC5, 0x43, 0xA5, 0xFE, 0x00, 0x8D,
		0xEE, 0xE3, 0xD3, 0xF0, 0x06, 0x00, 0x00, 0x00, 0x48, 0x6E, 0x3C, 0x77, 0x01, 0x00, 0x00, 0x00,
		0x9A, 0x8B, 0x13, 0x35, 0x96, 0x5D, 0xBD, 0x4F, 0x8E, 0x2D, 0xA2, 0x44, 0x02, 0x25, 0xF9, 0x3A,
		0x06, 0x00, 0x01, 0x00, 0x2C, 0x6E, 0x3C, 0x77, 0x02, 0x00, 0x00, 0x00, 0xE3, 0x28, 0x2F, 0x4A,
		0xB9, 0x53, 0x41, 0x44, 0xBA, 0x9C, 0xD6, 0x9D, 0x4A, 0x4A, 0x6E, 0x38, 0x06, 0x00, 0x02, 0x00,
		0x10, 0x6E, 0x3C, 0x77, 0x03, 0x00, 0x00, 0x00, 0x76, 0x6C, 0x67, 0x1F, 0xE1, 0x80, 0x39, 0x42,
		0x95, 0xBB, 0x83, 0xD0, 0xF6, 0xD0, 0xDA, 0x78, 0x06, 0x00, 0x03, 0x00, 0xF4, 0x6D, 0x3C, 0x77,
		0x04, 0x00, 0x00, 0x00, 0x12, 0x7A, 0x0F, 0x8E, 0xB3, 0xBF, 0xE8, 0x4F, 0xB9, 0xA5, 0x48, 0xFD,
		0x50, 0xA1, 0x5A, 0x9A, 0x0A, 0x00, 0x00, 0x00, 0xD0, 0x6D, 0x3C, 0x77, 0x18, 0x00, 0x1A, 0x00,
		0xC0, 0x6E, 0x3C, 0x77, 0x80, 0xA4, 0x41, 0x77, 0x20, 0x36, 0x3E, 0x77, 0x00, 0x00, 0x00, 0x00,
		0x50, 0x7E, 0x4B, 0x77, 0x08, 0x00, 0x0A, 0x00, 0x88, 0x76, 0x3C, 0x77, 0x08, 0x00, 0x0A, 0x00,
		0xA0, 0x77, 0x3C, 0x77, 0x02, 0x00, 0x04, 0x00, 0x00, 0x7A, 0x3C, 0x77, 0x06, 0x00, 0x08, 0x00,
		0x28, 0x7A, 0x3C, 0x77, 0x06, 0x00, 0x08, 0x00, 0x20, 0x7A, 0x3C, 0x77, 0x06, 0x00, 0x08, 0x00,
		0x18, 0x7A, 0x3C, 0x77, 0x06, 0x00, 0x08, 0x00, 0x30, 0x7A, 0x3C, 0x77, 0x0E, 0x00, 0x10, 0x00,
	};
	// 开始反汇编
	count = cs_disasm(handle,/*反汇编器句柄，从cs_open函数得到*/
		Opcode,/*需要反汇编的opcode的缓冲区首地址*/
		sizeof(Opcode),/*opcode的字节数*/
		0x401000,/*opcode的所在的内存地址*/
		0,/*需要反汇编的指令条数，如果是0，则反汇编出全部*/
		&pInsn/*反汇编输出*/
	);

	size_t j;
	for (j = 0; j < count; ++j)
	{
		printf("0x%" PRIx64":%s %s\n",
			pInsn[j].address,/*指令地址*/
			pInsn[j].mnemonic,/*指令操作码*/
			pInsn[j].op_str/*指令操作数*/
	
		);
	}

	// 释放保存指令空间
	cs_free(pInsn, count);
	// 关闭句柄
	cs_close(&handle);

	return 0;
}