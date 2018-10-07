#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "MyPass_Demo1.h"

/************定义全局变量**************/
//只能存100个数据
char website[100][32]; //站点
char username[100][32]; //用户名
char password[100][32]; //密码
char note[100][32]; //备注

int g_Num = 0; //记录密码个数


/***********功能实现*********/

//插入密码信息
void addPassWord()
{
	system("cls");
	printf("请输入<目标站点>:\n");
	scanf_s("%s", website[g_Num], 32);
	printf("请输入< 用户名 >:\n");
	scanf_s("%s", username[g_Num], 32);
	printf("请输入< 密  码 >:\n");
	scanf_s("%s", password[g_Num], 32);
	printf("请输入< 备  注 >:\n");
	scanf_s("%s", note[g_Num], 32);
	g_Num++;

}

//修改密码信息
void modifyPassWord()
{
	//1.输入用户需要修改的站点
	char szTemp[32] = { 0 };
	printf("请输入<目标站点>:\n");
	scanf_s("%s", szTemp, 32);
	//2.在站点数组中寻找目标站点
	int i = 0;
	int nFind = 0;
	for (; i < g_Num; ++i)
	{
		if (strcmp(szTemp, website[i]) == 0) //找到
		{
			nFind = 1;
			break;
		}
	}
	if (nFind)
	{
		printf("请输入< 用户名 >:\n");
		scanf_s("%s", username[i], 32);
		printf("请输入< 密  码 >:\n");
		scanf_s("%s", password[i], 32);
		printf("请输入< 备  注 >:\n");
		scanf_s("%s", note[i], 32);
		printf("修改成功\n");
	}
	else
	{
		printf("没有找到你输入的站点信息，请重新输入！\n");
	}
	system("pause");
}

//查找密码信息
//根据站点查找密码信息
void searchPassWordWeb()
{
	char szTemp[32] = { 0 };
	printf("请输入<目标站点>:\n");
	scanf_s("%s", szTemp, 32);
	
	int i = 0;
	int nFind = 0;
	for (; i < g_Num; ++i)
	{
		if (strcmp(szTemp, website[i]) == 0)
		{
			nFind = 1;  //找到
			break;
		}
	}
	if (nFind)
	{
		system("cls");
		printf("+-----------------------------------+\n");
		printf("<目标站点>:%s\n", website[i]);
		printf("< 用户名 >:%s\n", username[i]);
		printf("< 密  码 >:%s\n", password[i]);
		printf("< 备  注 >:%s\n", note[i]);
		printf("+-----------------------------------+\n");
		
	}
	else
	{
		printf("没有找到你输入的站点，请重新输入！\n");
	}
	system("pause");
}

//根据用户名查找密码信息
void searchPassWordUser()
{
	char szTemp[32] = { 0 };
	printf("请输入<用户名>:\n");
	scanf_s("%s", szTemp, 32);

	int i = 0;
	int nFind = 0;
	for (; i < g_Num; ++i)
	{
		if (strcmp(szTemp, username[i]) == 0)
		{
			nFind = 1;  //找到
			break;
		}
	}
	if (nFind)
	{
		system("cls");
		printf("+-----------------------------------+\n");
		printf("<目标站点>:%s\n", website[i]);
		printf("< 用户名 >:%s\n", username[i]);
		printf("< 密  码 >:%s\n", password[i]);
		printf("< 备  注 >:%s\n", note[i]);
		printf("+-----------------------------------+\n");
	}
	else
	{
		printf("没有找到你输入的用户名，请重新输入！\n");
	}
	system("pause");

}

//初始化密码本
void inital()
{
	system("title 欢迎使用密码本\n");
	//读取文件内容
	FILE * pfile = NULL;
	if ((fopen_s(&pfile, "data.txt", "rb")) != 0)
	{
		printf("文件打开失败!\n");
		return;

	}

	fscanf_s(pfile, "==共计:%3d条记录==================\n", &g_Num);
	int i = 0;
	for (; i < g_Num; ++i)
	{
		//fscanf_s 读取字符串，需要加上其大小
		fscanf_s(pfile, "--ID: %3d-------------\n", &i);
		fscanf_s(pfile, "目  标: %s \n", &website[i], 32);
		fscanf_s(pfile, "用户名: %s \n", &username[i], 32);
		fscanf_s(pfile, "密  码: %s \n", &password[i], 32);
		fscanf_s(pfile, "备  注: %s \n", &note[i], 32);
	}

	fclose(pfile);
	system("color 2");
	int number = 0;
	do 
	{
		system("cls");
		printf("\n 数据加载中 %d%%\n", number += 1);
		Sleep(30);
	} while (number < 100);
	
	Sleep(1000);
	printf("\n 数据加载完成!!!\n");
	system("pause");
}

//保存密码信息
void savePassWord() 
{
	
	FILE * pfile = NULL;
	fopen_s(&pfile, "data.txt", "wb");

	fprintf(pfile, "==共计:%3d条记录==================\n", g_Num);
	int i = 0;
	for (; i < g_Num; ++i)
	{
		fprintf(pfile, "--ID: %3d-------------\n", i);
		fprintf(pfile, "目  标: %s \n", website[i]);
		fprintf(pfile, "用户名: %s \n", username[i]);
		fprintf(pfile, "密  码: %s \n", password[i]);
		fprintf(pfile, "备  注: %s \n", note[i]);
	}

	fclose(pfile);
	printf("保存成功!\n");
	system("pause");

}

//删除密码信息
void delPassword()
{
	//输入用户需要删除的密码信息
	char szTemp[32] = { 0 };
	printf("请输入< 用户名 >:\n");
	scanf_s("%s", szTemp, 32);

	//在站点数组中寻找目标站点位置
	int nIndex = 0;
	int nFind = 0;
	for (; nIndex < g_Num; ++nIndex)
	{
		if (strcmp(szTemp, website[nIndex]) == 0)
		{
			//如果相等，直接删除
			nFind = 1;
			break;
		}
	}
	if (nFind)
	{
		for (int loc = nIndex; loc < g_Num - 1; ++loc)
		{
			//依次将后一个数据往前拷贝
			strcpy_s(website[loc], website[loc + 1]);
			strcpy_s(username[loc], username[loc + 1]);
			strcpy_s(password[loc], password[loc + 1]);
		}
		g_Num--;
	}
	else
	{
		printf("没有找到要删除数据,请重新输入!\n");
	}

	system("pause");
}
//显示所有密码信息
void showPassWord()
{
	system("cls");

	int i = 0;
	for (; i < g_Num; ++i)
	{
		printf("+------------第%d条信息--------------+\n", i + 1);
		printf("<目标站点>:%s\n", website[i]);
		printf("< 用户名 >:%s\n", username[i]);
		printf("< 密  码 >:%s\n", password[i]);
		printf("< 备  注 >:%s\n", note[i]);
		printf("+-----------------------------------+\n");

	}
	system("pause");
	
}

//打印菜单选项
void menu()
{
	system("cls");
	system("color 2"); //设置字体颜色
	system("title PassWord Online"); //修改标题
	printf("\n\n\n\n");
	printf("\t\t+--------------------密码本-------------------+\n");
	printf("\t\t|\t\t\t\t\t      |\n");
	printf("\t\t|\t\t 1. 插入密码信息\t      |\n\n");
	printf("\t\t|\t\t 2. 修改密码信息\t      |\n\n");
	printf("\t\t|\t\t 3. 查找密码信息\t      |\n\n");
	printf("\t\t|\t\t 4. 保存密码信息\t      |\n\n");
	printf("\t\t|\t\t 5. 删除密码信息\t      |\n\n");
	printf("\t\t|\t\t 6. 显示密码信息\t      |\n\n");
	printf("\t\t|\t\t 0. 退出\t\t      |\n");
	printf("\t\t|\t\t\t\t\t      |\n");
	printf("\t\t+---------------------------------------------+\n\n");
	printf("\t\t\t请选择[0～6]：");
}

//打印分级菜单
void divmenu()
{
	system("cls");
	system("color 2"); //设置字体颜色
	system("title 欢迎使用密码本"); //修改标题
	printf("\n\n\n\n");
	printf("\t\t+--------------------密码本-------------------+\n");
	printf("\t\t|\t\t\t\t\t      |\n");
	printf("\t\t|\t\t 1.根据站点查找密码信息\t      |\n\n");
	printf("\t\t|\t\t 2.根据用户名查找密码信息     |\n\n");
	printf("\t\t|\t\t 0.返回上一级\t              |\n");
	printf("\t\t|\t\t\t\t\t      |\n");
	printf("\t\t+---------------------------------------------+\n\n");
	printf("\t\t\t请选择[0～2]：");

}

//退出
void quit()
{
	exit(0);
}