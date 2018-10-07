#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "control.h"
#include "data.h"
#include <time.h>

#pragma comment(lib, "winmm.lib")

void welcometogame();       //游戏欢迎界面

/**
* 设置光标位置
*/
void setPos(int x, int y)
{
	COORD cci;
	cci.X = x;
	cci.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cci);
}


/**
* 文字颜色函数
*/
int setColor(int Color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);        //更改文字颜色
	return 0;
}


/*
*   蛇
*/
void printsnake()
{
	setPos(35, 1);
	setColor(6);
	printf("/^\\/^\\");      //蛇眼睛

	setPos(34, 2);
	printf("|__|  O|");      //蛇眼睛

	setPos(33, 2);
	setColor(2);
	printf("_");

	setPos(25, 3);
	setColor(12);
	printf("\\/");      	//蛇信

	setPos(31, 3);
	setColor(2);
	printf("/");

	setPos(37, 3);
	setColor(6);
	printf(" \\_/");     	//蛇眼睛

	setPos(41, 3);
	setColor(10);
	printf(" \\");

	setPos(26, 4);
	setColor(12);
	printf("\\____");   	//舌头

	setPos(32, 4);
	printf("_________/");

	setPos(31, 4);
	setColor(2);
	printf("|");

	setPos(43, 4);
	setColor(10);
	printf("\\");

	setPos(32, 5);
	setColor(2);
	printf("\\_______");    //蛇嘴

	setPos(44, 5);
	setColor(10);
	printf("\\");

	setPos(39, 6);
	printf("|     |                  \\");  //下面都是画蛇身

	setPos(38, 7);
	printf("/      /                   \\");

	setPos(37, 8);
	printf("/      /                    \\ \\");

	setPos(35, 9);
	printf("/      /                       \\ \\");

	setPos(34, 10);
	printf("/     /                          \\  \\");

	setPos(33, 11);
	printf("/     /             _----_         \\   \\");

	setPos(32, 12);
	printf("/     /           _-~      ~-_         |  |");

	setPos(31, 13);
	printf("(      (        _-~    _--_    ~-_     _/  |");

	setPos(32, 14);
	printf("\\     ~-____-~    _-~    ~-_    ~-_-~    /");

	setPos(33, 15);
	printf("~-_           _-~          ~-_       _-~");

	setPos(35, 16);
	printf("~--______-~                ~-___-~");
}

/*
*   游戏说明
*/
void explation()
{
	int i, j = 1;
	system("cls");
	setColor(13);
	setPos(44, 3);
	printf("游戏说明");
	setColor(2);
	for (i = 6; i <= 22; i++)   //输出上下边框===
	{
		for (j = 20; j <= 75; j++)  //输出左右边框||
		{
			setPos(j, i);
			if (i == 6 || i == 22) printf("=");
			else if (j == 20 || j == 75) printf("||");
		}
	}
	setColor(3);
	setPos(30, 8);
	printf("tip1: 不能穿墙，不能咬到自己");
	setColor(10);
	setPos(30, 11);
	printf("tip2: 用↑.↓.←.→分别控制蛇的移动");
	setColor(14);
	setPos(30, 14);
	printf("tip3: F1 为加速，F2 为减速");
	setColor(11);
	setPos(30, 17);
	printf("tip4: 按空格键暂停游戏，再按空格键继续");
	setColor(4);
	setPos(30, 20);
	printf("tip5: ESC ：退出游戏.space：暂停游戏");
	_getch();                //按任意键返回主界面
	system("cls");
	printsnake();
	welcometogame();
}

/**
* 地图编辑
*/
void editMap()
{
	system("cls");
	createMap();
	_getch();                //按任意键返回主界面
	system("cls");
	printsnake();
	welcometogame();
}

/**
* 开始界面
*/
void welcometogame()
{
	int n = 0;
	int i = 0, j = 0;
	setPos(43, 18);
	setColor(11);
	printf("贪 吃 蛇 游 戏");
	setColor(14);          			//黄色边框
	for (i = 20; i <= 26; i++)   	//输出上下边框┅
	{
		for (j = 27; j <= 74; j++)  //输出左右边框┇
		{
			setPos(j, i);
			if (i == 20 || i == 26)
			{
				printf("-");
			}
			else if (j == 27 || j == 74)
			{
				printf("|");
			}
		}
	}
	setColor(12);
	setPos(35, 22);
	printf("1.开始游戏");
	setPos(55, 22);
	printf("2.游戏说明");
	setPos(35, 24);
	printf("3.编辑地图");
	setPos(55, 24);
	printf("4.退出游戏");
	setPos(43, 27);
	setColor(3);
	printf("请选择[1 2 3 4]:[ ]\b\b");        //\b为退格，使得光标处于[]中间
	setColor(14);
	scanf_s("%d", &n);    		//输入选项
	switch (n)
	{
	case 1:
		system("cls");
		PlaySoundA("sound\\start.wav", NULL, SND_ASYNC | SND_NODEFAULT); //播放背景音效
		createMap();        //创建地图
		initsnake();        //初始化蛇身
		createfood();       //创建食物
		keyboardControl();	//按键控制
		break;
	case 2:
		PlaySoundA("sound\\end.wav", NULL, SND_ASYNC | SND_NODEFAULT); //播放背景音效
		explation();      	//游戏说明函数
		break;
	case 3:
		editMap();		    //编辑地图
		break;
	case 4:
		exit(0);     		//退出游戏
		break;
	default:				//输入非1~3之间的选项
		setColor(12);
		setPos(40, 28);
		printf("请输入1~4之间的数!");
		_getch();			//输入任意键返回
		system("cls");		//清屏
		printsnake();
		welcometogame();
	}
}


/**
*  游戏界面右侧的得分和小提示
*/
void scoreandtips()
{
	File_out();
	setPos(64, 4);
	setColor(11);
	printf("☆最高记录☆：%d", HighScore);
	setPos(64, 8);
	setColor(14);
	printf("得分：%d  ", score);
	setColor(13);
	setPos(73, 11);
	printf("小 提 示");
	setPos(60, 13);
	setColor(6);
	printf("╬ ┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅ ╬");
	setPos(60, 25);
	printf("╬ ┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅ ╬");
	setColor(3);
	setPos(64, 14);
	printf("每个食物得分：%d分", add);
	setPos(64, 16);
	printf("不能穿墙，不能咬到自己");
	setPos(64, 18);
	printf("用↑ ↓ ← →分别控制蛇的移动");
	setPos(64, 20);
	printf("F1 为加速，F2 为减速");
	setPos(64, 22);
	printf("space：暂停游戏");
	setPos(64, 24);
	printf("ESC ：退出游戏");
}

/**
* 失败界面
*/
void Lostdraw()
{
	system("cls");
	int i;
	setPos(45, 2);
	setColor(6);
	printf("\\\\\\|///");
	setPos(43, 3);
	printf("\\\\");
	setPos(47, 3);
	setColor(15);
	printf(".-.-");
	setPos(54, 3);
	setColor(6);
	printf("//");

	setPos(44, 4);
	setColor(14);
	printf("(");

	setPos(47, 4);
	setColor(15);
	printf(".@.@");

	setPos(54, 4);
	setColor(14);
	printf(")");

	setPos(17, 5);
	setColor(11);
	printf("+------------------------");

	setPos(35, 5);
	setColor(14);
	printf("oOOo");

	setPos(39, 5);
	setColor(11);
	printf("----------");

	setPos(48, 5);
	setColor(14);
	printf("(_)");

	setPos(51, 5);
	setColor(11);
	printf("----------");

	setPos(61, 5);
	setColor(14);
	printf("oOOo");

	setPos(65, 5);
	setColor(11);
	printf("-----------------+");

	for (i = 6; i <= 19; i++)        //竖边框
	{
		setPos(17, i);
		printf("|");
		setPos(82, i);
		printf("|");
	}

	setPos(17, 20);
	printf("+-----------------------------------");

	setPos(52, 20);
	setColor(14);
	printf("☆☆☆〃");

	setPos(60, 20);
	setColor(11);
	printf("----------------------+");

}

/**
* 边框下面的分支选项
*/
void choose()
{
	int n;
	setPos(25, 23);
	setColor(12);
	printf("我要重新玩一局-------1");
	setPos(52, 23);
	printf("不玩了，退出吧-------2");
	setPos(46, 25);
	setColor(11);
	printf("选择：");
	scanf_s("%d", &n);
	switch (n)
	{
	case 1:
		system("cls");          //清屏
		score = 0;                //分数归零
		sleeptime = 200;			//设定初始速度
		add = 10;				//使add设定为初值，吃一个食物得分10，然后累加
		printsnake();           //返回欢迎界面
		welcometogame();
		break;
	case 2:
		exit(0);                //退出游戏
		break;
	default:
		setPos(35, 27);
		setColor(12);
		printf("※※你的输入有误，请重新输入※※");
		system("pause >nul");
		endgame();
		choose();
		break;
	}

}














