// 密码本.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "data.h"
#include <stdlib.h>
#include "ctrl.h"
#include <string.h>
#include <windows.h>
#include <conio.h>
#pragma comment(lib, "winmm.lib")


//void setColor(int color);
//void setCurson(int x, int y);

//定义要显示的菜单   
char *menu[] =
{
	"\t\t\t\t 1. 查看所有密码\t      \n\n",
	"\t\t\t\t 2. 新增密码\t\t      \n\n",
	"\t\t\t\t 3. 删除密码\t\t      \n\n",
	"\t\t\t\t 4. 修改密码\t\t      \n\n",
	"\t\t\t\t 5. 查询密码\t\t      \n\n",
	"\t\t\t\t 6. 生成文件\t\t      \n\n",
	"\t\t\t\t 7. 退出\t\t      \n",
};
//光标 
CONSOLE_CURSOR_INFO cci;
//定义默认的坐标位置       
COORD pos = { 0,0 };
//显示菜单   
void showmenu(HANDLE hOut, char **menu, int size, int index);
//获取用户输入   
int  get_userinput(int *index, int size);
int main()
{
	int ret;
	int index = 0;
	PPASSWORD pPwd = nullptr;  //密码数组
	int nCount = 0;  //当前密码个数
	g_nNum = 0;  //当前密码本容量

	readInfo(&pPwd, &nCount); //从本地文件中读取密码，用于初始化密码本

	HANDLE hOut;
	//获取当前的句柄---设置为标准输出句柄   
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//获取光标信息  
	GetConsoleCursorInfo(hOut, &cci);
	//设置光标大小     
	cci.dwSize = 1;
	//设置光标不可见 FALSE     
	cci.bVisible = 0;
	//设置(应用)光标信息  
	SetConsoleCursorInfo(hOut, &cci);
	PlaySoundA("sound\\xiao.wav", NULL, SND_ASYNC | SND_NODEFAULT); //播放背景乐
	while (1)
	{
		showmenu(hOut, menu, _countof(menu),index);
		ret = get_userinput(&index, _countof(menu));
		if (ret == ESC)
			break;
		if (ret == ENTER)
		{
			switch (index)
			{
			case 查看所有:
				searchAllInfo(pPwd, nCount);
				break;
			case 新增:
				insertInfo(pPwd, &nCount);
				break;
			case 删除:
				deleteInfo(pPwd, &nCount);
				break;
			case 修改:
				alterInfo(pPwd, nCount);
				break;
			case 查询:
				searchInfo(pPwd, nCount);
				break;
			case 保存文本:
				saveInfoA(pPwd, nCount);
				break;
			case 退出:
				exit(0);
				break;
			default:
				break;
			}
			system("cls");
		}
	}
	return 0;
}

void showmenu(HANDLE hOut, char **menu, int size, int index)
{
	int i;
	system("cls");
	//设置显示的文本的颜色   
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | 0x8);
	//初始化控制台显示的X,Y轴的坐标   
	pos.X = 30;
	pos.Y = 0;
	//设置显示到控制台终端的具体位置   
	SetConsoleCursorPosition(hOut, pos);

	for (i = 0; i < size; i++)
	{
		//如果i==index表示在当前选项的位置，默认初始化显示是第一项，显示为红色，  
		//当按下上下按键选择的时候，光标会移动，也就看到了列表选择的现象   
		if (i == index)
		{
			//红色   
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | 0x8);
			pos.X = 25;
			pos.Y = 5 + i;
			//设置光标坐标  
			SetConsoleCursorPosition(hOut, pos);
			printf("%s", menu[i]);
		}
		//否则显示为白色   
		else
		{
			//白色   
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			pos.X = 30;
			pos.Y = 5 + i;
			//设置光标坐标  
			SetConsoleCursorPosition(hOut, pos);    //设置光标坐标  
			printf("%s", menu[i]);
		}
	}
	//刷新标准输出缓冲区   
	fflush(stdout);
}

//获取用户输入的接口   
int  get_userinput(int *index, int size)
{
	int ch;
	ch = _getch();
	switch (ch)
	{
		//上   
		//如果选择上，那么光标向上移动   
	case UP: if (*index > 0)  *index -= 1;  break;
		//下   
		//如果选择下，那么光标向下移动   
	case DOWN:if (*index < size - 1)  *index += 1;  break;
		//回车   
	case ENTER: return ENTER;
		//ESC  
	case ESC: return ESC;
	}
	return 0;
}

int main11()
{
	PPASSWORD pPwd= nullptr;  //密码数组
	int nCount = 0;  //当前密码个数
	g_nNum = 0;  //当前密码本容量

	//从本地文件中读取密码，用于初始化密码本
	readInfo(&pPwd, &nCount); 
	/*
	while (true)
	{
		PlaySoundA("sound\\xiao.wav", NULL, SND_ASYNC | SND_NODEFAULT); //播放背景乐
		
		setColor(12);
		system("title 欢迎使用密码本"); //修改标题
		printf("\n\n\n\n");
		printf("\t\t+--------------------密码本-------------------+\n");
		printf("\t\t|\t\t\t\t\t      |\n");
		setColor(5);
		printf("\t\t|\t\t 1. 查看所有密码\t      |\n\n");
		setColor(1);
		printf("\t\t|\t\t 2. 新增密码\t\t      |\n\n");
		setColor(3);
		printf("\t\t|\t\t 3. 删除密码\t\t      |\n\n");
		setColor(7);
		printf("\t\t|\t\t 4. 修改密码\t\t      |\n\n");
		setColor(11);
		printf("\t\t|\t\t 5. 查询密码\t\t      |\n\n");
		setColor(6);
		printf("\t\t|\t\t 6. 生成文件\t\t      |\n\n");
		setColor(10);
		printf("\t\t|\t\t 0. 退出\t\t      |\n");
		printf("\t\t|\t\t\t\t\t      |\n");
		printf("\t\t+---------------------------------------------+\n\n");
		setColor(4);
		printf("\t\t\t请选择[0～6]：[ ]\b\b");

		int op;
		scanf_s("%d", &op);

		switch (op)
		{
		case 查看所有:
			searchAllInfo(pPwd, nCount);
			break;
		case 新增:
			insertInfo(pPwd, &nCount);
			break;
		case 删除:
			deleteInfo(pPwd, &nCount);
			break;
		case 修改:
			alterInfo(pPwd, nCount);
			break;
		case 查询:
			searchInfo(pPwd, nCount);
			break;
		case 保存文本:
			saveInfoA(pPwd, nCount);
			break;
		case 退出:
			exit(0);
			break;
		default:
			break;
		}
		system("cls");
	}
	*/

    return 0;
}

//设置颜色
void setColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/*

//设置光标
void setCurson(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
*/