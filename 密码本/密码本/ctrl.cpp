#include "stdafx.h"
#include "ctrl.h"
#include <stdlib.h>
#include <string.h>


//查看所有密码
void searchAllInfo(PASSWORD pwd[], int nCount)
{
	printf("==共计: %2d 条记录==============\n", nCount);
	for (size_t i = 0; i < nCount; i++)
	{
		// typedef unsigned int     size_t;
		printf("--ID: %2d -------------------\n", i);
		printf("目  标:%s \n", pwd[i].webSite);
		printf("用户名:%s \n", pwd[i].userName);
		printf("密  码:%s \n", pwd[i].passWord);
		printf("备  注:%s \n", pwd[i].note);
	}

	system("pause");
}

//新增密码
void insertInfo(PASSWORD pwd[], int *pnCount)
{
	int i = *pnCount;
	if (i < 0 || i >= NUM)
	{
		printf("插入位置异常:%d \n", i);
		return;
	}
	printf("请输入<目标站点>\n");
	scanf_s("%s", pwd[i].webSite, 100);
	printf("请输入< 用户名 >\n");
	scanf_s("%s", pwd[i].userName, 100);
	printf("请输入<  密码  >\n");
	scanf_s("%s", pwd[i].passWord, 100);
	printf("请输入<  备注  >\n");
	scanf_s("%s", pwd[i].note, 100);

	(*pnCount)++;
	saveInfoB(pwd, *pnCount);

	system("pause");
	system("cls");
}
//删除密码
void deleteInfo(PASSWORD pwd[], int *pnCount)
{
	int i = -1;
	printf("请输入待删除项<密码ID>\n");
	scanf_s("%d", &i);
	if (i < 0 || i > NUM)
	{
		printf("删除位置异常:%d \n", i);
		return;
	}

	for (size_t j = i; j < *pnCount - 1; j++)
	{
		pwd[j] = pwd[j + 1];
	}

	(*pnCount)--;
	saveInfoB(pwd, *pnCount);

	system("pause");
	system("cls");
}

//修改密码
void alterInfo(PASSWORD pwd[], int nCount)
{
	int i = -1;
	printf("请输入待修该项<密码ID>\n");
	scanf_s("%d", &i);
	if (i < 0 || i >= NUM)
	{
		printf("修改位置异常:%d \n", i);
		return;
	}

	printf("--ID: %3d ---------------------\n", i);
	printf("目  标:%s \n", pwd[i].webSite);
	printf("用户名:%s \n", pwd[i].userName);
	printf("密  码:%s \n", pwd[i].passWord);
	printf("备  注:%s \n", pwd[i].note);
	printf("~修改~\n");
	printf("请输入<目标站点>\n");
	scanf_s("%s", pwd[i].webSite, 100);
	printf("请输入< 用户名 >\n");
	scanf_s("%s", pwd[i].userName, 100);
	printf("请输入<  密码  >\n");
	scanf_s("%s", pwd[i].passWord, 100);
	printf("请输入<  备注  >\n");
	scanf_s("%s", pwd[i].note, 100);

	saveInfoB(pwd, nCount);

	system("pause");
	system("cls");
	
}

//查询密码
void searchInfo(PASSWORD pwd[], int nCount)
{
	char key[100] = { 0 };
	printf("请输入查找密码的关键字:\n");
	scanf_s("%s", key, 100);

	//找出所有匹配项
	int findPwd[100] = { 0 };
	int nfindCount = 0;
	for (size_t i = 0; i < nCount; i++)
	{
		if (strstr(pwd[i].webSite, key))
		{
			findPwd[nfindCount++] = i;
			continue;
		}
		if (strstr(pwd[i].userName, key))
		{
			findPwd[nfindCount++] = i;
			continue;
		}
		if (strstr(pwd[i].passWord, key))
		{
			findPwd[nfindCount++] = i;
			continue;
		}
		if (strstr(pwd[i].note, key))
		{
			findPwd[nfindCount++] = i;
			continue;
		}
	}//for

	//打印所有匹配项
	printf("==共计匹配:%2d 条记录===============\n", nfindCount);
	for (size_t i = 0; i < nfindCount; i++)
	{
		int pos = findPwd[i];
		printf("--ID:%2d -----------------------\n", pos);
		printf("目  标:%s \n", pwd[i].webSite);
		printf("用户名:%s \n", pwd[i].userName);
		printf("密  码:%s \n", pwd[i].passWord);
		printf("备  注:%s \n", pwd[i].note);
	}
	system("pause");
	system("cls");
}

//保存密码(文本的方式)
void saveInfoA(PASSWORD pwd[], int nCount)
{
	/*
	读写文件的固定套路
	1. 打开文件
	2. 读写文件
	3. 关闭文件
	*/

	//打开文件
	FILE *pFile;
	errno_t err = fopen_s(&pFile, "data.txt", "w");
	if (err != 0)
	{
		return;
	}
	//读写文件
	fprintf(pFile, "==共计:%2d 条记录=================\n", nCount);
	for (size_t i = 0; i < nCount; i++)
	{
		fprintf(pFile, "--ID:%2d ---------------------\n", i);
		fprintf(pFile, "目  标: %s \n", pwd[i].webSite);
		fprintf(pFile, "用户名: %s \n", pwd[i].userName);
		fprintf(pFile, "密  码: %s \n", pwd[i].passWord);
		fprintf(pFile, "备  注: %s \n", pwd[i].note);
	}

	//关闭文件
	fclose(pFile);

	printf("保存文本密码成功，使用后请尽快删除!!\n");
	system("pause");
}

//保存密码(二进制的方式)
void saveInfoB(PASSWORD pwd[], int nCount)
{
	//打开文件
	FILE  *pFile;
	errno_t err = fopen_s(&pFile, "data.i", "wb");
	if (err != 0)
	{
		return;
	}

	//读写文件
	fwrite(&nCount, sizeof(nCount), 1, pFile); //当前密码个数
	fwrite(&g_nNum, sizeof(g_nNum), 1, pFile); //当前密码本容量

	//先加密(使用中间文件)，再存储
	int byteSize = sizeof(PASSWORD) * g_nNum;
	char *buf = (char *)malloc(byteSize);
	memcpy_s(buf, byteSize, pwd, byteSize);

	for (size_t i = 0; i < byteSize; i++)
	{
		buf[i] -= 1;
	}
	/*函数原型
	#include <stdio.h>
	int fwrite(const void *buffer, size_t size, size_t count, FILE *stream);

	fwrite()函数从数组buffer(缓冲区)中, 
	写count个大小为size(大小)的对象到stream(流)指定的流. 
	返回值是已写的对象的数量. 
	*/
	size_t rlen = fwrite(buf, sizeof(PASSWORD), g_nNum, pFile); //密码数组
	if (rlen != g_nNum)
	{
		printf("写入密码文件错误\n");
	}
	else
	{
		printf("文件写入成功!!\n");
	}

	//关闭文件
	fclose(pFile);
	free(buf);

}

//从本地文件中读取密码，用于初始化密码本
void readInfo(PPASSWORD *ppPwd, int *pnCount)
{
	//打开文件
	FILE *pFile;
	errno_t err = fopen_s(&pFile, "data.i", "rb");
	if (err != 0)
	{
		*ppPwd = (PPASSWORD)malloc(sizeof(PASSWORD) * NUM);
		g_nNum = NUM;
		return;
	}

	//读写文件
	//先读取，再解密
	fread(pnCount, sizeof(int), 1, pFile);  
	fread(&g_nNum, sizeof(g_nNum), 1, pFile);
	*ppPwd = (PPASSWORD)malloc(sizeof(PASSWORD) * g_nNum);
	memset(*ppPwd, 0, sizeof(PASSWORD) * g_nNum);
	size_t rlen = fread(*ppPwd, sizeof(PASSWORD), g_nNum, pFile); //密码数组
	if (rlen != g_nNum)
	{
		printf("读取密码文件错误\n");
		fclose(pFile);
		return;
	}
	int byteSize = sizeof(PASSWORD) * g_nNum;
	char *buf = (char *)*ppPwd;
	for (size_t i = 0; i < byteSize; i++)
	{
		buf[i] += 1;
	}

	//关闭文件
	fclose(pFile);

}

//改变控制台字体颜色
