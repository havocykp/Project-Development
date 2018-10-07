#pragma once
#include "control.h"
#include "data.h"
#include "interface.h"
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

typedef struct snake 		//蛇身的一个节点
{
	int x;
	int y;
	struct snake *next;
}snake;

snake *head, *food;			  //蛇头指针，食物指针
snake *q;					  //遍历蛇的时候用到的指针

int score = 0, add = 10;
int HighScore = 0;
int status, sleeptime = 200;
int endgamestatus = 0;

/**
* 创建地图
*/
void createMap()
{
	int i;
	for (i = 0; i < 58; i += 2)	   //打印上下边框
	{
		setPos(i, 0);
		setColor(2);			   //绿色的边框
		printf("▓");
		setPos(i, 26);
		printf("▓");
	}
	for (i = 1; i < 26; i++)		//打印左右边框
	{
		setPos(0, i);
		printf("▓");
		setPos(56, i);
		printf("▓");
	}
}


/**
* 初始化蛇身，画蛇身
*/
void initsnake()
{
	snake *tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));//从蛇尾开始，头插法，以x,y设定开始的位置
	tail->x = 24;         	//蛇的初始位置（24,5）
	tail->y = 5;
	tail->next = NULL;
	for (i = 1; i < 3; i++)       //设置蛇身，长度为3
	{
		head = (snake*)malloc(sizeof(snake)); //初始化蛇头
		head->next = tail;        //蛇头的下一位为蛇尾
		head->x = 24 + 2 * i;     //设置蛇头位置
		head->y = 5;
		tail = head;              //蛇头变成蛇尾，然后重复循环
	}
	while (tail != NULL)		  //从头到尾，输出蛇身
	{
		setPos(tail->x, tail->y);
		setColor(14);
		printf("●");          //输出蛇身，蛇身使用●组成
		tail = tail->next;    //蛇头输出完毕，输出蛇头的下一位，一直输出到蛇尾
	}
}


/**
* 随机出现食物
*/
void createfood()
{
	snake *food_1;
	srand((unsigned)time(NULL));        	  //初始化随机数
	food_1 = (snake*)malloc(sizeof(snake));   //初始化food_1
	 while ((food_1->x % 2) != 0)    		  
	{
		food_1->x = rand() % 52 + 2;          //食物随机出现
	}
	food_1->y = rand() % 24 + 1;
	q = head;
	while (q->next == NULL)
	{
		if (q->x == food_1->x && q->y == food_1->y) //判断蛇身是否与食物重合
		{
			free(food_1);               //如果蛇身和食物重合，那么释放食物指针
			createfood();               //重新创建食物
		}
		q = q->next;
	}
	setPos(food_1->x, food_1->y);
	food = food_1;
	setColor(12);
	printf("★");           //输出食物
}

/**
*  加速，蛇吃到食物会自动提速，并且按F1会加速
*/
void speedup()
{
	if (sleeptime >= 50)
	{
		sleeptime = sleeptime - 10;
		add = add + 2;
	}
}

/**
*  加速，按F2会减速
*/
void speeddown()
{
	if (sleeptime < 350)               //如果时间间隔小于350
	{
		sleeptime = sleeptime + 30;     //时间间隔加上30
		add = add - 2;                  //每吃一次食物的得分减2

	}
}

/**
* 判断是否咬到了自己
*/
int biteself()
{
	snake *self;            //定义self为蛇身上的一个节点
	self = head->next;      //self是蛇头之外的蛇身上的节点
	while (self != NULL)
	{
		if (self->x == head->x && self->y == head->y)    //如果self和蛇身上的节点重合
		{
			return 1;       //返回1
		}
		self = self->next;
	}
	return 0;
}

/**
* 设置蛇撞墙的情况
*/
void cantcrosswall()
{
	if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26) //如果蛇头碰到了墙壁
	{
		endgamestatus = 1;      //返回第一种情况
		endgame();              //出现游戏结束界面
	}
}


/**
* 结束游戏
*/
void endgame()
{
	system("cls");
	if (endgamestatus == 1)
	{

		Lostdraw();
		setPos(35, 9);
		setColor(12);
		printf("嗨，老兄，你要去哪？撞到墙了。游戏结束！");
	}
	else if (endgamestatus == 2)
	{

		Lostdraw();
		setPos(35, 9);
		setColor(12);
		printf("Fuck You，咬到自己了。游戏结束！");
	}
	else if (endgamestatus == 3)
	{
		Lostdraw();
		setPos(40, 9);
		setColor(12);
		printf("你已经结束了游戏。");
	}
	setPos(43, 12);
	setColor(13);
	printf("你的得分是 %d", score);

	if (score >= HighScore)
	{
		setColor(10);
		setPos(33, 16);
		printf("创纪录啦！最高分被你刷新啦，真棒！！！");
		File_in();              //把最高分写进文件
	}
	else
	{
		setColor(10);
		setPos(33, 16);
		printf("继续努力吧~ 你离最高分还差：%d", HighScore - score);
	}
	choose();
}

/**
*  控制方向
*/
void snakemove()	//蛇前进,上Up,下Ddown,左Left,右Right
{
	snake * nexthead;
	cantcrosswall();
	nexthead = (snake*)malloc(sizeof(snake));		//为下一步开辟空间
	if (status == DIR_UP)
	{
		nexthead->x = head->x;        //向上前进时，x坐标不动，y坐标-1
		nexthead->y = head->y - 1;
		nexthead->next = head;
		head = nexthead;
		q = head;                 //指针q指向蛇头
		if (nexthead->x == food->x && nexthead->y == food->y)	//如果下一个有食物 下一个位置的坐标和食物的坐标相同
		{

			while (q != NULL)
			{
				setPos(q->x, q->y);
				setColor(14);
				printf("●");          //原来食物的位置，从★换成●
				q = q->next;          //指针q指向的蛇身的下一位也执行循环里的操作

			}
			score = score + add;      //吃了一个食物，在总分上加上食物的分
			speedup();
			createfood();             //创建食物
		}
		else
		{
			while (q->next->next != NULL)	//如果没遇到食物
			{
				setPos(q->x, q->y);
				setColor(14);
				printf("●");              //蛇正常往前走，输出当前位置的蛇身
				q = q->next;              //继续输出整个蛇身
			}
			setPos(q->next->x, q->next->y);  //经过上面的循环，q指向蛇尾，蛇尾的下一位，就是蛇走过去的位置
			setColor(3);
			printf("  ");
			free(q->next);        //进行输出之后，释放指向下一位的指针
			q->next = NULL;       //指针下一位指向空
		}
	}
	if (status == DIR_DOWN)
	{
		nexthead->x = head->x;        //向下前进时，x坐标不动，y坐标+1
		nexthead->y = head->y + 1;
		nexthead->next = head;
		head = nexthead;
		q = head;
		if (nexthead->x == food->x && nexthead->y == food->y)  //有食物
		{

			while (q != NULL)
			{
				setPos(q->x, q->y);
				setColor(14);
				printf("●");
				q = q->next;
			}
			score = score + add;
			speedup();
			createfood();
		}
		else                               //没有食物
		{
			while (q->next->next != NULL)
			{
				setPos(q->x, q->y);
				setColor(14);
				printf("●");
				q = q->next;
			}
			setPos(q->next->x, q->next->y);
			setColor(3);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == DIR_LEFT)
	{
		nexthead->x = head->x - 2;        //向左前进时，x坐标向左移动-2，y坐标不动
		nexthead->y = head->y;
		nexthead->next = head;
		head = nexthead;
		q = head;
		if (nexthead->x == food->x && nexthead->y == food->y)//有食物
		{
			while (q != NULL)
			{
				setPos(q->x, q->y);
				setColor(14);
				printf("●");
				q = q->next;
			}
			score = score + add;
			speedup();
			createfood();
		}
		else                                //没有食物
		{
			while (q->next->next != NULL)
			{
				setPos(q->x, q->y);
				setColor(14);
				printf("●");
				q = q->next;
			}
			setPos(q->next->x, q->next->y);
			setColor(3);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == DIR_RIGHT)
	{
		nexthead->x = head->x + 2;        //向右前进时，x坐标向右移动+2，y坐标不动
		nexthead->y = head->y;
		nexthead->next = head;
		head = nexthead;
		q = head;
		if (nexthead->x == food->x && nexthead->y == food->y)//有食物
		{
			while (q != NULL)
			{
				setPos(q->x, q->y);
				setColor(14);
				printf("●");
				q = q->next;
			}
			score = score + add;
			speedup();
			createfood();
		}
		else                                         //没有食物
		{
			while (q->next->next != NULL)
			{
				setPos(q->x, q->y);
				setColor(14);
				printf("●");
				q = q->next;
			}
			setPos(q->next->x, q->next->y);
			setColor(3);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (biteself() == 1)       //判断是否会咬到自己
	{
		endgamestatus = 2;
		endgame();
	}
}

/**
* 控制键盘按键
*/
void keyboardControl()
{
	status = DIR_RIGHT;       //初始蛇向右移动
	while (1)
	{
		scoreandtips();
		if (GetAsyncKeyState(VK_UP) && status != DIR_UP)  //GetAsyncKeyState函数用来判断函数调用时指定虚拟键的状态
		{
			status = DIR_UP;           //如果蛇不是向下前进的时候，按上键，执行向上前进操作
		}
		else if (GetAsyncKeyState(VK_DOWN) && status != DIR_UP)     //如果蛇不是向上前进的时候，按下键，执行向下前进操作
		{
			status = DIR_DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != DIR_RIGHT)  //如果蛇不是向右前进的时候，按左键，执行向左前进
		{
			status = DIR_LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && status != DIR_LEFT)  //如果蛇不是向左前进的时候，按右键，执行向右前进
		{
			status = DIR_RIGHT;
		}
		if (GetAsyncKeyState(VK_SPACE))		//按暂停键，执行pause暂停函数
		{
			while (1)
			{
				Sleep(300);
				if (GetAsyncKeyState(VK_SPACE))      //按空格键暂停
				{
					break;
				}

			}
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
		{
			endgamestatus = 3;    //按esc键，直接到结束界面
			break;
		}
		else if (GetAsyncKeyState(VK_F1))    //按F1键，加速
		{
			speedup();
		}
		else if (GetAsyncKeyState(VK_F2))    //按F2键，减速
		{
			speeddown();

		}
		Sleep(sleeptime);
		snakemove();
	}
}


/**
* 储存最高分进文件
*/
void File_in()
{
	FILE *fp = NULL;
	fopen_s(&fp, "save.txt", "w+"); //以读写的方式建立一个名为save.txt的文件
	fprintf(fp, "%d", score);       //把分数写进文件中
	fclose(fp);                     //关闭文件
}


/**
* 在文件中读取最高分
*/
void File_out()
{
	FILE *fp = NULL;
	fopen_s(&fp, "save.txt", "a+");     //打开文件save.txt
	fscanf_s(fp, "%d", &HighScore);     //把文件中的最高分读出来
	fclose(fp);                         //关闭文件
}

/**
* 实现一个能在屏幕任何位置输出字符串的函数
*/
// void WriteChar(int x, int y, char *szInfo, int co)
// {
// 	COORD pos = { x * 2, y };
// 	//获得输出句柄
// 	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
// 	//设置颜色
// 	SetConsoleTextAttribute(hOut, co);
// 	//设置光标位置
// 	SetConsoleCursorPosition(hOut, pos);
// 	//输出字符串
// 	printf("%s", szInfo);
// }

/**
* 输出调试坐标信息
*/
// void OutputPos(int nX, int nY)
// {
// 	char szBuf[100] = { 0 };
// 	sprintf_s(szBuf, sizeof(szBuf), "x = %2d, y = %2d ", nX / 2, nY);
// 	SetConsoleTitleA(szBuf);
// 	WriteChar(0, 0, szBuf, 0x50);
// }

/**
* 鼠标事件处理函数
*/
// void MouseEventProc(MOUSE_EVENT_RECORD mer)
// {
// 	switch (mer.dwEventFlags)
// 	{
// 	case 0:
// 		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
// 		{
// 			WriteChar(0, 0, "左键单击", 0x50);
// 		}
// 		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
// 		{
// 			WriteChar(0, 0, "右键单击", 0x50);
// 		}
// 		break;
// 	case DOUBLE_CLICK:
// 		WriteChar(0, 0, "双击", 0x50);
// 		break;
// 	case MOUSE_MOVED:
// 		OutputPos(mer.dwMousePosition.X, mer.dwMousePosition.Y);
// 		break;
// 	}
// }



