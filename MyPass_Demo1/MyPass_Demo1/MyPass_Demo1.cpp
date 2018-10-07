// MyPass_Demo1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"   //在新建的cpp文件中，都需要包含
#include "control.h"
#include "MyPass_Demo1.h"
#include <windows.h>
#include <conio.h>


int main()
{
	int nChoice = 0;
	inital(); //初始化数据
	while (true)
	{
		//打印菜单选项
		menu();
		scanf_s("%d", &nChoice);  //读取输入
		switch (nChoice)
		{
		case 插入:
			addPassWord();
			break;
		case 修改:
			system("cls");
			modifyPassWord();
			break;
		case 查找:
		{
			int nNum = 0;
			while (true)
			{
				menu();
				scanf_s("%d", &nChoice);
				divmenu();
				scanf_s("%d", &nNum);
				switch (nNum)
				{
				case 1:
					searchPassWordWeb();
					break;
				case 2:
					searchPassWordUser();
					break;
				case 0:
					menu();
					break;
				default:
					break;
				}
			}	
			
		}
			break;
		case 保存:
			savePassWord();
			break;
		case 删除:
			system("cls");
			delPassword();
			break;
		case 显示:
			showPassWord();
			break;
		case 退出: 
			quit();
			break;
		default:
			break;

		}
	}


    return 0;
}



