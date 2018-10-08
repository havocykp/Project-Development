// 项目1-1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <time.h>
#include <conio.h>


//宏定义
#define FrameX 13  //游戏窗口左上角的x轴坐标
#define FrameY 3   //游戏窗口左上角的y轴坐标
#define Frame_height 20 //游戏窗口高度
#define Frame_width 18  //游戏窗口宽度

//定义全局变量
int i, j, Temp, Temp1, Temp2; //Temp,Temp1,Temp2用于记住和转换方块变量的值
int a[80][80] = { 0 }; //标记游戏屏幕的图案，2， 1， 0分别表示该位置为游戏边框、方块、无图案；初始化为无图案
int b[4]; //标记4个口方块：1表示有方块，0表示没有方块

struct Tetris  //声明俄罗斯方块的结构体
{
	int x; //中心方块的x轴坐标
	int y; //中心方块的y轴坐标
	int flag; //标记方块类型的序号
	int next; //下一个俄罗斯方块类型的序号
	int speed; //俄罗斯方块移动的速度
	int number; //产生俄罗斯方块的个数
	int score; //游戏的分数
	int level; //游戏的等级
};
HANDLE hOut; //控制台句柄

//函数声明
void setPos(int x, int y); //光标移动到指定位置
void drawGameFrame();  //绘制游戏边框
void flag(struct Tetris *); //随机产生方块类型的序号
void makeTetris(struct Tetris *); //制作俄罗斯方块
void printTetris(struct Tetris *); //打印俄罗斯方块
void cleanTetris(struct Tetris *); //清除俄罗斯方块的痕迹
int  ifMove(struct Tetris *);  //判断是否能移动
void del_fullLine(struct Tetris *); //判断是否满行，并删除满行的俄罗斯方块
void gamePlay();  //开始游戏
void regulation(); //游戏规则
void explation();  //按键说明
void welcome();   //欢迎界面
void replay(struct Tetris *);  //重新开始游戏
void title();  //欢迎界面上方的标题
void flower(); //欢迎界面上的字符装饰花
void close();  //关闭游戏

//获取屏幕光标位置
void setPos(int x, int y) {
	COORD pos;
	pos.X = x; //横坐标
	pos.Y = y; //纵坐标
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//文字颜色函数
int color(int c) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
	return 0;
}

//制作游戏窗口
void drawGameFrame() {
	setPos(FrameX + Frame_width - 7, FrameY - 1); //打印游戏名称
	color(11);
	printf("趣味俄罗斯方块");
	setPos(FrameX + 2 * Frame_width + 3, FrameY + 7); //打印选择菜单
	color(2);
	printf("*******************");
	setPos(FrameX + 2 * Frame_width + 13, FrameY + 7); //打印选择菜单
	color(3);
	printf("下一出现方块：");
	setPos(FrameX + 2 * Frame_width + 3, FrameY + 13);
	color(2);
	printf("******************");
	setPos(FrameX + 2 * Frame_width + 3, FrameY + 17);
	color(14);
	printf("↑键：旋转");
	setPos(FrameX + 2 * Frame_width + 3, FrameY + 19);
	printf("空格：暂停游戏");
	setPos(FrameX + 2 * Frame_width + 3, FrameY + 15);
	printf("Esc：退出游戏");
	setPos(FrameX, FrameY);
	color(12);
	printf("╔"); //打印框角
	setPos(FrameX + 2 * Frame_width - 2, FrameY);
	printf("╗");
	setPos(FrameX, FrameY + Frame_height);
	printf("╚");
	setPos(FrameX + 2 * Frame_width - 2, FrameY + Frame_height);
	printf("╝");

	a[FrameX][FrameY + Frame_height] = 2; //记住该处已有图案
	a[FrameX + 2 * Frame_width - 2][FrameY + Frame_height] = 2;

	for (i = 2; i < 2 * Frame_width - 2; i += 2) {
		setPos(FrameX + i, FrameY);
		printf("═"); //打印上横框
	}
	for (i = 2; i < 2 * Frame_width - 2; i += 2) {
		setPos(FrameX + i, FrameY + Frame_height);
		printf("═"); //打印下横框
		a[FrameX + i][FrameY + Frame_height] = 2; //记住下横框有图案
	}
	for (i = 1; i < Frame_height; i++) {
		setPos(FrameX, FrameY + i);
		printf("║"); //打印左竖框
		a[FrameX][FrameY + i] = 2; //记住左竖框有图案
	}
	for (i = 1; i < Frame_height; i++) {
		setPos(FrameX + 2 * Frame_width - 2, FrameY + i);
		printf("║"); //打印右竖框
		a[FrameX + 2 * Frame_width - 2][FrameY + i] = 2; //记住右竖框有图案
	}
}

//制作俄罗斯方块
void makeTetris(struct Tetris *tetris) {
	a[tetris->x][tetris->y] = b[0]; //中心方块位置的图形状态
	switch (tetris->flag) {
	case 1:
	{
		color(10);
		a[tetris->x][tetris->y - 1] = b[1];
		a[tetris->x + 2][tetris->y - 1] = b[2];
		a[tetris->x + 2][tetris->y] = b[3];
		break;
	}
	case 2:
	{
		color(13);
		a[tetris->x - 2][tetris->y] = b[1];
		a[tetris->x + 2][tetris->y] = b[2];
		a[tetris->x + 4][tetris->y] = b[3];
		break;
	}
	case 3:
	{
		color(13);
		a[tetris->x][tetris->y - 1] = b[1];
		a[tetris->x][tetris->y - 2] = b[2];
		a[tetris->x][tetris->y + 1] = b[3];
		break;
	}
	case 4:
	{
		color(11);
		a[tetris->x - 2][tetris->y] = b[1];
		a[tetris->x + 2][tetris->y] = b[2];
		a[tetris->x][tetris->y + 1] = b[3];
		break;
	}
	case 5:
	{
		color(11);
		a[tetris->x][tetris->y - 1] = b[1];
		a[tetris->x][tetris->y + 1] = b[2];
		a[tetris->x - 2][tetris->y] = b[3];
		break;
	}
	case 6:
	{
		color(11);
		a[tetris->x][tetris->y - 1] = b[1];
		a[tetris->x - 2][tetris->y] = b[2];
		a[tetris->x + 2][tetris->y] = b[3];
		break;
	}
	case 7:
	{
		color(11);
		a[tetris->x][tetris->y - 1] = b[1];
		a[tetris->x][tetris->y + 1] = b[2];
		a[tetris->x + 2][tetris->y] = b[3];
		break;
	}
	case 8:
	{
		color(14);
		a[tetris->x][tetris->y + 1] = b[1];
		a[tetris->x - 2][tetris->y] = b[2];
		a[tetris->x + 2][tetris->y + 1] = b[3];
		break;
	}
	case 9:
	{
		color(14);
		a[tetris->x][tetris->y - 1] = b[1];
		a[tetris->x - 2][tetris->y] = b[2];
		a[tetris->x - 2][tetris->y + 1] = b[3];
		break;
	}
	case 10:
	{
		color(14);
		a[tetris->x][tetris->y - 1] = b[1];
		a[tetris->x - 2][tetris->y] = b[2];
		a[tetris->x - 2][tetris->y + 1] = b[3];
		break;
	}
	case 11:
	{
		color(14);
		a[tetris->x][tetris->y + 1] = b[1];
		a[tetris->x - 2][tetris->y - 1] = b[2];
		a[tetris->x - 2][tetris->y] = b[3];
		break;
	}
	case 12:
	{
		color(12);
		a[tetris->x][tetris->y - 1] = b[1];
		a[tetris->x][tetris->y + 1] = b[2];
		a[tetris->x - 2][tetris->y - 1] = b[3];
		break;
	}
	case 13:
	{
		color(12);
		a[tetris->x - 2][tetris->y] = b[1];
		a[tetris->x + 2][tetris->y - 1] = b[2];
		a[tetris->x + 2][tetris->y] = b[3];
		break;
	}
	case 14:
	{
		color(12);
		a[tetris->x][tetris->y - 1] = b[1];
		a[tetris->x][tetris->y + 1] = b[2];
		a[tetris->x + 2][tetris->y + 1] = b[3];
		break;
	}
	case 15:
	{
		color(12);
		a[tetris->x - 2][tetris->y] = b[1];
		a[tetris->x - 2][tetris->y + 1] = b[2];
		a[tetris->x + 2][tetris->y] = b[3];
		break;
	}
	case 16:
	{
		color(12);
		a[tetris->x][tetris->y + 1] = b[1];
		a[tetris->x][tetris->y - 1] = b[2];
		a[tetris->x + 2][tetris->y - 1] = b[3];
		break;
	}
	case 17:
	{
		color(12);
		a[tetris->x - 2][tetris->y] = b[1];
		a[tetris->x + 2][tetris->y + 1] = b[2];
		a[tetris->x + 2][tetris->y] = b[3];
		break;
	}
	case 18:
	{
		color(12);
		a[tetris->x][tetris->y - 1] = b[1];
		a[tetris->x][tetris->y + 1] = b[2];
		a[tetris->x - 2][tetris->y + 1] = b[3];
		break;
	}
	case 19:
	{
		color(12);
		a[tetris->x - 2][tetris->y] = b[1];
		a[tetris->x - 2][tetris->y - 1] = b[2];
		a[tetris->x + 2][tetris->y] = b[3];
		break;
	}

	}
}

//打印俄罗斯方块
void printTetris(struct Tetris *tetris) {
	for (i = 0; i < 4; i++) {
		b[i] = 1;  //数组b[4]的每个元素的值都为1
	}
	makeTetris(tetris);  //制作游戏窗口
	for (i = tetris->x - 2; i <= tetris->x + 4; i += 2) {
		for (j = tetris->y - 2; j <= tetris->y + 1; j++) {
			if (a[i][j] == 1 && j > FrameY) {
				setPos(i, j);
				printf("■");  //打印边框内的方块
			}
		}
	}
	//打印菜单信息
	setPos(FrameX + 2 * Frame_width + 3, FrameY + 1);
	color(4);
	printf("level：");
	color(12);
	printf("%d", tetris->level);
	setPos(FrameX + 2 * Frame_width + 3, FrameY + 3);
	color(4);
	printf("score：");
	color(12);
	printf("%d", tetris->score);
	setPos(FrameX + 2 * Frame_width + 3, FrameY + 5);
	color(4);
	printf("speed：");
	color(12);
	printf("%dms", tetris->speed);
}

//判断是否可移动
int ifMove(struct Tetris *tetris) {
	if (a[tetris->x][tetris->y] != 0) //当中心方块位置上有图案时，返回值为0，即不可移动
	{
		return 0;
	}
	else
	{
		if ((tetris->flag == 1 && (a[tetris->x][tetris->y - 1] == 0 &&
			a[tetris->x + 2][tetris->y - 1] == 0 && a[tetris->x + 2][tetris->y] == 0)) ||
			(tetris->flag == 2 && (a[tetris->x - 2][tetris->y] == 0 &&
			a[tetris->x + 2][tetris->y] == 0 && a[tetris->x + 4][tetris->y] == 0)) ||
			(tetris->flag == 3 && (a[tetris->x][tetris->y - 1] == 0 &&
			a[tetris->x][tetris->y - 2] == 0 && a[tetris->x][tetris->y + 1])) ||
			(tetris->flag == 4 && (a[tetris->x - 2][tetris->y] == 0 &&
			a[tetris->x + 2][tetris->y] == 0 && a[tetris->x][tetris->y + 1])) ||
			(tetris->flag == 5 && (a[tetris->x][tetris->y - 1] == 0 &&
			a[tetris->x][tetris->y + 1] == 0 && a[tetris->x - 2][tetris->y] == 0)) ||
			(tetris->flag == 6 && (a[tetris->x][tetris->y - 1] == 0 &&
			a[tetris->x - 2][tetris->y] == 0 && a[tetris->x + 2][tetris->y] == 0)) ||
			(tetris->flag == 7 && (a[tetris->x][tetris->y - 1] == 0 &&
			a[tetris->x][tetris->y + 1] == 0 && a[tetris->x][tetris->y + 1] == 0)) ||
			(tetris->flag == 8 && (a[tetris->x][tetris->y + 1] == 0 &&
			a[tetris->x - 2][tetris->y] == 0 && a[tetris->x + 2][tetris->y + 1] == 0)) ||
			(tetris->flag == 9 && (a[tetris->x][tetris->y - 1] == 0 &&
			a[tetris->x - 2][tetris->y] == 0 && a[tetris->x - 2][tetris->y + 1] == 0)) ||
			(tetris->flag == 10 && (a[tetris->x][tetris->y - 1] == 0 &&
			a[tetris->x - 2][tetris->y] == 0 && a[tetris->x - 2][tetris->y + 1] == 0)) ||
			(tetris->flag == 11 && (a[tetris->x][tetris->y + 1] == 0 &&
			a[tetris->x - 2][tetris->y - 1] == 0 && a[tetris->x - 2][tetris->y] == 0)) ||
			(tetris->flag == 12 && (a[tetris->x][tetris->y - 1] == 0 &&
			a[tetris->x][tetris->y + 1] == 0 && a[tetris->x - 2][tetris->y - 1] == 0)) ||
			(tetris->flag == 13 && (a[tetris->x - 2][tetris->y] == 0 &&
			a[tetris->x + 2][tetris->y - 1] == 0 && a[tetris->x + 2][tetris->y] == 0)) ||
			(tetris->flag == 14 && (a[tetris->x][tetris->y - 1] == 0 &&
			a[tetris->x][tetris->y + 1] == 0 && a[tetris->x + 2][tetris->y + 1] == 0)) ||
			(tetris->flag == 15 && (a[tetris->x - 2][tetris->y] == 0 &&
			a[tetris->x - 2][tetris->y + 1] == 0 && a[tetris->x + 2][tetris->y] == 0)) ||
			(tetris->flag == 16 && (a[tetris->x][tetris->y + 1] == 0 &&
			a[tetris->x][tetris->y - 1] == 0 && a[tetris->x + 2][tetris->y - 1] == 0)) ||
			(tetris->flag == 17 && (a[tetris->x - 2][tetris->y] == 0 &&
			a[tetris->x + 2][tetris->y + 1] == 0 && a[tetris->x + 2][tetris->y] == 0)) ||
			(tetris->flag == 18 && (a[tetris->x][tetris->y - 1] == 0 &&
			a[tetris->x][tetris->y + 1] == 0 && a[tetris->x - 2][tetris->y + 1] == 0)) ||
			(tetris->flag == 19 && (a[tetris->x - 2][tetris->y] == 0 &&
			a[tetris->x - 2][tetris->y - 1] == 0 && a[tetris->x + 2][tetris->y] == 0)))
		{
			return 1;
		}
	}
	return 0;
}

//随机产生俄罗斯方块类型的序号
void flag(struct Tetris * tetris) {

	tetris->number++; //记住产生俄罗斯方块的个数
	srand((unsigned)time(NULL)); //初始化随机数
	if (tetris->number == 1) {
		tetris->flag = rand() % 19 + 1; //记住第一个方块的序号
	}
	tetris->next = rand() % 19 + 1;  //记住下一个方块的序号
}

//清除俄罗斯方块的痕迹
void cleanTetris(struct Tetris *tetris) {
	for (i = 0; i < 4; i++) {
		b[i] = 0; //数组b[4]的每个元素的值都为0
	}
	makeTetris(tetris);  //制作俄罗斯方块
	for (i = tetris->x - 2; i <= tetris->x + 4; i += 2) {
		for (j = tetris->y - 2; j <= tetris->y + 1; j++) {
			if (a[i][j] == 0 && j > FrameY) {
				setPos(i, j);
				printf("  "); //清除方块
			}
		}
	}
}

//判断是否满行并删除满行的俄罗斯方块
void del_fullLine(struct Tetris *tetris) //当某行有Frame_with - 2个方块时，则满行消除
{
	int k, del_rows = 0; //分别用于记录某行方块的个数和删除方块的行数的变量
	for (j = FrameY + Frame_height - 1; j >= FrameY + 1; j--)
	{
		k = 0;
		for (i = FrameX + 2; i < FrameX + 2 * Frame_width - 2; i += 2)
		{
			if (a[i][j] == 1) //竖坐标依次从下往上，横坐标依次由左至右判断是否满行
			{
				k++;  //记录此行方块的个数
				if (k == Frame_width - 2) // 如果满行
				{
					//删除满行的方块
					for (k = FrameX + 2; k < FrameX + 2 * Frame_width - 2; k += 2)
					{
						a[k][j] = 0;
						setPos(k, j);
						printf("  ");
					}
					for (k = j - 1; k > FrameY; k--)
					{
						//如果删除行以上的位置有方块，则先清除，再将方块下移一个位置
						for (i = FrameX + 2; i < FrameX + 2 * Frame_width - 2; i += 2)
						{
							if (a[i][k] == 1)
							{
								a[i][k] = 0;
								setPos(i, k);
								printf("  ");
								a[i][k + 1] = 1;
								setPos(i, k + 1);
								printf("■");
							}
						}
					}
					j++; //方块下移后，重新判断删除行是否满行
					del_rows++; //记录删除方块的行数
				}
			}
		}
	}
	tetris->score += 100 * del_rows; //每删除一行，得100分
	if (del_rows > 0 && (tetris->score % 1000 == 0 || tetris->score / 1000 > tetris->level - 1))
	{
		//如果得1000分即累计删除10行，速度加快20ms并升一级
		tetris->speed -= 20;
		tetris->level++;
	}
}

//开始游戏
void gamePlay() {
	struct Tetris t, *tetris = &t;
	char ch;
	tetris->number = 0;
	tetris->speed = 300;
	tetris->score = 0;
	tetris->level = 1;
	while (1)
	{
		flag(tetris);
		Temp = tetris->flag;
		tetris->x = FrameX + 2 * Frame_width + 6;
		tetris->y = FrameY + 10;
		tetris->flag = tetris->next;
		printTetris(tetris);
		tetris->x = FrameX + Frame_width;
		tetris->y = FrameY - 1;
		tetris->flag = Temp;
		while (1)
		{
		label:printTetris(tetris);
			Sleep(tetris->speed);
			cleanTetris(tetris);
			Temp1 = tetris->x;
			Temp2 = tetris->flag;
			if (_kbhit())
			{
				ch = _getch();
				if (ch == 75) //按👈
				{
					tetris->x -= 2;
				}
				if (ch == 77) //按👉
				{
					tetris->x += 2;
				}
				if (ch == 80) //按👇
				{
					if (ifMove(tetris) != 0)
					{
						tetris->y += 2;
					}
					if (ifMove(tetris) == 0)
					{
						tetris->y = FrameY + Frame_height - 2;
					}
				}
				if (ch == 72) //按👆
				{
					if (tetris->flag >= 2 && tetris->flag <= 3)
					{
						tetris->flag++;
						tetris->flag %= 2;
						tetris->flag += 2;
					}
					if (tetris->flag >= 4 && tetris->flag <= 7)
					{
						tetris->flag++;
						tetris->flag %= 4;
						tetris->flag += 4;
					}
					if (tetris->flag >= 8 && tetris->flag <= 11)
					{
						tetris->flag++;
						tetris->flag %= 4;
						tetris->flag += 8;
					}
					if (tetris->flag >= 12 && tetris->flag <= 15)
					{
						tetris->flag++;
						tetris->flag %= 4;
						tetris->flag += 12;
					}
					if (tetris->flag >= 16 && tetris->flag <= 19)
					{
						tetris->flag++;
						tetris->flag %= 4;
						tetris->flag += 12;
					}
					if (tetris->flag >= 16 && tetris->flag <= 19)
					{
						tetris->flag++;
						tetris->flag %= 4;
						tetris->flag += 16;
					}
				}
				if (ch == 32) //按空格键，暂停
				{
					printTetris(tetris);
					while (1)
					{
						if (_kbhit())  //再按空格键，继续游戏
						{
							ch = _getch();
							if (ch == 32)
							{
								goto label;
							}
						}
					}
				}
				if (ch == 27)
				{
					system("cls");
					memset(a, 0, 6400 * sizeof(int));
					welcome();
				}
				if (ifMove(tetris) == 0)  //如果不可动，上面操作无效
				{
					tetris->x = Temp1;
					tetris->flag = Temp2;
				}
				else  //如果可动，执行操作
				{
					goto label;
				}
			}
			tetris->y++;  //如果没有操作指令，方块向下移动
			if (ifMove(tetris) == 0) //如果向下移动且不可动，方块放在此处
			{
				tetris->y--;
				printTetris(tetris);
				del_fullLine(tetris);
				break;
			}
		}
		for (i = tetris->y - 2; i < tetris->y + 2; i++) //游戏结束条件：方块触到框顶位置
		{
			if (i == FrameY)
			{
				system("cls");
				printf("   \n");
				color(12);
				printf("\t\t\t■■■■   ■     ■   ■■     \n");
				printf("\t\t\t■         ■■   ■   ■  ■   \n");
				printf("\t\t\t■■■     ■  ■ ■   ■   ■  \n");
				printf("\t\t\t■         ■   ■■   ■  ■   \n");
				printf("\t\t\t■■■■   ■     ■   ■■     \n");
				setPos(17, 18);
				color(14);
				printf("我要重新玩一局-------1");
				setPos(44, 18);
				printf("不玩了，退出吧-------2\n");
				int n;
				setPos(32, 20);
				printf("选择【1/2】：");
				color(11);
				scanf_s("%d", &n);
				switch (n)
				{
				case 1:
					system("cls");
					replay(tetris); //重新开始
					break;
				case 2:
					exit(0);
					break;
				}
			}
		}
		tetris->flag = tetris->next;  //清除下一个俄罗斯方块的图形（右边窗口）
		tetris->x = FrameX + 2 * Frame_width + 6;
		tetris->y = FrameY + 10;
		cleanTetris(tetris);

	}
}

//重新游戏
void replay(struct Tetris *)
{
	system("cls");  //清屏
	memset(a, 0, 6400 * sizeof(int)); 
	drawGameFrame(); //制作游戏窗口
	gamePlay();  //开始游戏
}

int main()
{
	title(); //欢迎界面标题
	flower();
	welcome();

    return 0;
}

//欢迎界面
void welcome()
{
	int n;
	int i, j = 1;
	color(14);  //黄色边框
	for (i = 9; i <= 20; i++) //输出上下边框
	{
		for (j = 15; j <= 60; j++) //输出左右边框
		{
			setPos(j, i);
			if (i == 9 || i == 20) printf("═");
			else if (j == 15 || j == 59) printf("║");
		}
	}
	color(12);
	setPos(25, 12);
	printf("1.开始游戏");
	setPos(40, 12);
	printf("2.按键说明");
	setPos(25, 17);
	printf("3.游戏规则");
	setPos(40, 17);
	printf("4.退出");
	setPos(21, 22);
	color(3);
	printf("请选择[1 2 3 4]:[ ]\b\b");
	color(14);
	scanf_s("%d", &n);  //输入选项
	switch (n)
	{
	case 1:
		system("cls");
		drawGameFrame(); //制作游戏窗口
		gamePlay();   //开始游戏
		break;
	case 2:
		explation();  //游戏按键说明
		break;
	case 3:
		regulation(); //游戏规则
		break;
	case 4:
		close();  //关闭游戏函数
		break;
	}
}

void title()
{
	color(15);
	setPos(24, 3);
	printf("趣  味  俄  罗  斯  方  块\n");
	color(11);
	setPos(18, 5);
	printf("■");
	setPos(18, 6);
	printf("■■");
	setPos(18, 7);
	printf("■");

	color(14);
	setPos(26, 6);
	printf("■■");
	setPos(28, 7);
	printf("■■");//

	color(10);
	setPos(36, 6);
	printf("■■");
	setPos(36, 7);
	printf("■■");

	color(13);
	setPos(45, 5);
	printf("■");
	setPos(45, 6);
	printf("■");
	setPos(45, 7);
	printf("■");
	setPos(45, 8);
	printf("■");

	color(12);
	setPos(56, 6);
	printf("■");//
	setPos(52, 7);
	printf("■■■");
}

void flower()
{
	setPos(66, 11);   	//确定屏幕上要输出的位置
	color(12);			//设置颜色
	printf("(_)");  	//红花上边花瓣

	setPos(64, 12);
	printf("(_)");      //红花左边花瓣

	setPos(68, 12);
	printf("(_)");      //红花右边花瓣

	setPos(66, 13);
	printf("(_)");      //红花下边花瓣

	setPos(67, 12);      //红花花蕊
	color(6);
	printf("@");

	setPos(72, 10);
	color(13);
	printf("(_)");      //粉花左边花瓣

	setPos(76, 10);
	printf("(_)");      //粉花右边花瓣

	setPos(74, 9);
	printf("(_)");      //粉花上边花瓣

	setPos(74, 11);
	printf("(_)");      //粉花下边花瓣

	setPos(75, 10);
	color(6);
	printf("@");        //粉花花蕊

	setPos(71, 12);
	printf("|");      	//两朵花之间的连接

	setPos(72, 11);
	printf("/");      	//两朵花之间的连接

	setPos(70, 13);
	printf("\\|");       //注意、\为转义字符。想要输入\，必须在前面需要转义

	setPos(70, 14);
	printf("`|/");

	setPos(70, 15);
	printf("\\|");

	setPos(71, 16);
	printf("| /");

	setPos(71, 17);
	printf("|");

	setPos(67, 17);
	color(10);
	printf("\\\\\\\\");      //草地

	setPos(73, 17);
	printf("//");

	setPos(67, 18);
	color(2);
	printf("^^^^^^^^");
}

//退出
void close()
{
	exit(0);
}

void explation()
{
	int i, j = 1;
	system("cls");
	color(13);
	setPos(32, 3);
	printf("按键说明");
	color(2);
	for (i = 6; i <= 16; i++)
	{
		for (j = 15; j <= 60; j++)
		{
			setPos(j, i);
			if (i == 6 || i == 16) printf("═");
			else if (j == 15 || j == 59) printf("║");
		}
	}
	color(3);
	setPos(18, 7);
	printf("tip1: 玩家可以通过 ← →方向键来移动方块");
	color(10);
	setPos(18, 9);
	printf("tip2: 通过 ↑使方块旋转");
	color(14);
	setPos(18, 11);
	printf("tip3: 通过 ↓加速方块下落");
	color(11);
	setPos(18, 13);
	printf("tip4: 按空格键暂停游戏，再按空格键继续");
	color(4);
	setPos(18, 15);
	printf("tip5: 按ESC退出游戏");
	_getch();                //按任意键返回主界面
	system("cls");
	main();
}

//游戏规则
void regulation()
{
	int i, j = 1;
	system("cls");
	color(13);
	setPos(34, 3);
	printf("游戏规则");
	color(2);
	for (i = 6; i <= 18; i++)
	{
		for (j = 12; j <= 70; j++)
		{
			setPos(j, i);
			if (i == 6 || i == 18) printf("═");
			else if (j == 12 || j == 69) printf("║");
		}
	}

	color(12);
	setPos(16, 7);
	printf("tip1: 不同形状的小方块从屏幕上方落下，玩家通过调整");
	setPos(22, 9);
	printf("方块的位置和方向，使他们在屏幕底部拼出完整的");
	setPos(22, 11);
	printf("一条或几条");
	color(14);
	setPos(16, 13);
	printf("tip2: 每消除一行，积分增加100");
	color(11);
	setPos(16, 15);
	printf("tip3: 每累计1000分，会提升一个等级");
	color(10);
	setPos(16, 17);
	printf("tip4: 提升等级会使方块下落速度加快，游戏难度加大");
	_getch();                //按任意键返回主界面
	system("cls");
	welcome();
	
}