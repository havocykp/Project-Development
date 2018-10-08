// 项目2-2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <time.h>
#include <math.h>
#include <conio.h>

//定义全局变量
int step = 0; //已执行步数
int score = 0; //存储游戏分数
long int Time; //游戏运行时间
int BOX[4][4] = { 0,0,0,0,//游戏中的16个格子
				  0,0,0,0,
				  0,0,0,0,
				  0,0,0,0 };
HANDLE hOut; //控制台句柄


//函数声明
void setPos(int x, int y);//获取屏幕光标位置					  
int color(int c);//文字颜色函数	 
int textColor(int number);//根据数字修改颜色				  
void drawTheGameBox();//绘制游戏界面 4*4的网格				 
int *add(int item[]); //合并数字  
int ifMove(int item[]);//判断能否移动数字，返回1能动，0不能动					  
int if2n(int x); //判断x是否是2的n次方，若是，返回1，否则返回0			 
int ifup();//判断是否能够上移，可以上移，返回1，不能移动，则返回0  
int ifdown(); //判断是否能够下移  可以下移，则返回1  不能移动，则返回0		  
int ifleft();//判断是否能否左移  可以左移，则返回1， 不能移动，返回0	 
int ifright();//判断是否能够右移  可以右移，则返回1  不能移动，则返回0		  
int BOXmax();//返回棋盘最大数		 
int gameFaile();//判断是否失败	
int gameWin();//判断是否胜利	 
int keyboardControl(int key); //键盘控制移动						
void close();  //退出		   
void gamePlay();//开始游戏			
void replay();//重新开始游戏		 
void title(); //设置标题		  
void choice();//菜单选项
void choice(); //菜单选项


//主函数
int main()
{
// 	for(;;)
// 	if (_kbhit())
// 	{
// 		char str = _getch();
// 		//printf("%c", str);
// 		printf("%d", str);
// 	}

	title(); //打印标题
	choice(); //菜单选项

	return 0;
}


//获取屏幕光标位置
void setPos(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//文字颜色函数
int color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
	return 0;
}

//根据数字修改颜色
int textColor(int number)
{
	switch (number)
	{
	case 2:
		return color(12);
		break;
	case 4:
		return color(11);
		break;
	case 8:
		return color(10);
		break;
	case 16:
		return color(14);
		break;
	case 32:
		return color(6);
		break;
	case 64:
		return color(5);
		break;
	case 128:
		return color(4);
		break;
	case 256:
		return color(3);
		break;
	case 512:
		return color(2);
		break;
	case 1024:
		return color(9);
		break;
	case 2048:
		return color(7);
		break;
	}
	return 0;
}

//绘制游戏界面 4*4的网格
void drawTheGameBox()
{
	int i, j, k;
	setPos(16, 1); //屏幕坐标位置
	color(11);  //淡浅绿色
	printf("游戏分数：%ld", score);
	color(13); //粉色
	setPos(42, 1); //屏幕坐标位置
	printf("执行步数: %d\n", step);
	color(14); //黄色
	for (j = 2; j <= 22; j += 5) //打印棋盘边框
	{
		setPos(15, j);
		for (k = 1; k < 42; k++)
		{
			printf("-");
		}
		printf("\n");
	}
	for (i = 3; i < 7; i++)
	{
		setPos(15, i);
		printf("|         |         |         |         | ");
	}
	for (i = 8; i < 12; i++)
	{
		setPos(15, i);
		printf("|         |         |         |         | ");
	}
	for (i = 13; i < 17; i++)
	{
		setPos(15, i);
		printf("|         |         |         |         | ");
	}
	for (i = 18; i < 22; i++)
	{
		setPos(15, i);
		printf("|         |         |         |         | ");
	}
	setPos(44, 23);
	color(10); //绿色
	printf("已用时：%lld s", time(NULL) - Time); //输出游戏运行时间
}

//合并数字
int *add(int item[])
{
	int i = 0, j = 0;
	int tep[4] = { 0, 0, 0, 0 }, tmp[4] = { 0, 0, 0, 0 };
	for (i = 0; i < 4; i++)
	{
		if (item[i] != 0) //如果这个格子里有数字
		{
			tep[j++] = item[i];
		}
	}
	//把两个相邻的相同的数字加起来
	for (i = 0; i < 4; i++)
	{
		if (tep[i] == tep[i + 1]) //如果两个数字相同，则合并
		{
			tep[i] *= 2; //一个格子中的数字翻倍，另一个为空
			tep[i + 1] = 0;
			score = score + tep[i]; //加分，加的分数为消除的数字 *2
		}
	}
	j = 0;
	for (i = 0; i < 4; i++)
	{
		if (tep[i] != 0) //如果格子里有数字
		{
			tmp[j++] = tep[i];
		}
	}

// 	for (i = 0;i < 4;++i)
// 		item[i] = tmp[i];
	return (int *)(&tmp);
}

//判断能否移动数字，返回1能动，0不能动
int ifMove(int item[])
{
	int i = 0;
	for (i = 0; i < 3; i++)
	{
		//如果不为空并且相邻相同
		if (item[i] != 0 && item[i] == item[i + 1])
		{
			return 1;
		}
		//如果为空，相邻不同
		if (item[i] == 0 && item[i + 1] != 0)
		{
			return 1;
		}
	}
	return 0;  //不能合并，返回0
}

//判断x是否是2的n次方，若是，返回1，否则返回0
int if2n(int x)
{
	int flag = 0;
	int n;
	int N = 1;
	for (n = 1; n <= 11; n++) //2的11次方是2048
	{
		if (x == pow(2, n)) //计算2的n次方
		{
			flag = 1;
			if (n > N)
				N = n;
			return flag;
		}
	}
	return flag;
}

//判断是否能够上移，可以上移，返回1，不能移动，则返回0
int ifup()
{
	int i, j;
	int flag = 0; //定义标志变量，只有0或1
	for(j = 0; j < 4; j++)
		for (i = 0; i < 3; i++)
		{
			//如果上下两个格子相加是2的n次方，并且下面的格子中有数
			if ((if2n(BOX[i][j] + BOX[i + 1][j]) == 1) && BOX[i + 1][j])
			{
				flag = 1;  //可以上移
			}
		}
	return flag;
}

//判断是否能够下移  可以下移，则返回1  不能移动，则返回0
int ifdown()
{
	int i, j;
	int flag = 0;
	for(j = 0; j < 4; j++)
		for (i = 3; i > 0; i--)
		{
			//如果上下两个格子相加是2的n次方，并且上面的格子中有数
			if ((if2n(BOX[i][j] + BOX[i - 1][j]) == 1) && BOX[i - 1][j])
			{
				flag = 1; //可以下移
			}
		}
	return flag;
}

//判断是否能否左移  可以左移，则返回1， 不能移动，返回0
int ifleft()
{
	int i, j;
	int flag = 0;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 3; j++)
		{
			//如果左右两个格子相加是2的n次方，并且右面的格子中有数
			if ((if2n(BOX[i][j] + BOX[i][j + 1]) == 1) && BOX[i][j + 1])
			{
				flag = 1; //可以左移
			}
		}
	return flag;
}

//判断是否能够右移  可以右移，则返回1  不能移动，则返回0
int ifright()
{
	int i, j;
	int flag = 0;
	for(i = 0; i < 4; i++)
		for (j = 3; j > 0; j--)
		{
			//如果左右两个格子相加是2的n次方，并且左面的格子中有数
			if ((if2n(BOX[i][j] + BOX[i][j - 1]) == 1) && BOX[i][j - 1])
			{
				flag = 1; //可以右移
			}
		}
	return flag;
}

//返回棋盘最大数
int BOXmax()
{
	int max = BOX[0][0]; //初始化BOX数组
	int i, j;
	for (i = 0; i < 4; i++)  //遍历整个数组
	{
		for (j = 0; j < 4; j++)
		{
			if (BOX[i][j] > max) //如果数组中有数值大于max
			{
				max = BOX[i][j]; //将数组中的值赋值给max
			}
		}
	}
	return max; //返回棋盘中的最大值
}

//判断是否失败
int gameFaile()
{
	int flag = 0;
	int max;
	//当上下左右不能移动的时候，游戏失败
	if (ifup() + ifdown() + ifleft() + ifright() == 0)
	{
		system("cls");
		setPos(34, 3);
		color(14);
		printf("合并出的最大数是：");
		setPos(52, 3);
		color(11);
		max = BOXmax();
		printf("%d", max);
		setPos(19, 6);
		color(4);   //暗红色
		printf("     ■■■■■    ■         ■    ■■     \n");  //输出end字符画
		setPos(19, 7);
		printf("     ■            ■■       ■    ■  ■   \n");
		setPos(19, 8);
		printf("     ■            ■ ■      ■    ■    ■   \n");
		setPos(19, 9);
		printf("     ■            ■  ■     ■    ■     ■  \n");
		setPos(19, 10);
		printf("     ■■■■      ■   ■    ■    ■      ■  \n");
		setPos(19, 11);
		printf("     ■            ■    ■   ■    ■     ■  \n");
		setPos(19, 12);
		printf("     ■            ■     ■  ■    ■    ■   \n");
		setPos(19, 13);
		printf("     ■            ■      ■ ■    ■  ■  \n");
		setPos(19, 14);
		printf("     ■■■■■    ■       ■■    ■■     \n");
		setPos(34, 17);
		color(13);
		printf("无法移动，游戏失败！");
		flag = 1;
	}
	return flag; //flag的值正常是0，返回1则表示游戏失败
}

//判断是否胜利
int gameWin()
{
	int flag = 0; 
	//如果棋盘中的最大值为2048，达到目标，则游戏胜利
	if (BOXmax() == 2048)
	{
		setPos(1, 6);
		color(2);   //暗绿色
		printf("     ■             ■             ■       ■■■■■       ■          ■  \n");  //输出胜利win的字符画
		setPos(1, 7);
		printf("      ■           ■■           ■            ■           ■ ■       ■  \n");
		setPos(1, 8);
		printf("       ■         ■  ■         ■             ■           ■  ■      ■  \n");
		setPos(1, 9);
		printf("        ■       ■    ■       ■              ■           ■   ■     ■   \n");
		setPos(1, 10);
		printf("         ■     ■      ■     ■               ■           ■    ■    ■  \n");
		setPos(1, 11);
		printf("          ■   ■        ■   ■                ■           ■     ■   ■ \n");
		setPos(1, 12);
		printf("           ■ ■          ■ ■                 ■           ■      ■  ■    \n");
		setPos(1, 13);
		printf("            ■■          ■■                  ■           ■       ■ ■  \n");
		setPos(1, 14);
		printf("             ■            ■               ■■■■■       ■          ■   \n");
		setPos(35, 17);
		color(13);
		printf("胜利啦，你真棒！！！");
		flag = 1;
	}
	return flag; //flag的值正常是0，返回1则表示游戏胜利
}

//键盘控制移动
int keyboardControl(int key)
{
	int i = 0, j = 0;
	int *p;
	int tp[4] = { 0, 0, 0, 0 };
	//left = 75, up = 72, down = 80, right = 77
	switch (key)
	{
	
	case 72://up
		j = 0;
		for (i = 0; i < 4; i++)
		{
			tp[0] = BOX[0][i]; //把一列数移到中间变量
			tp[1] = BOX[1][i];
			tp[2] = BOX[2][i];
			tp[3] = BOX[3][i];
			p = add(tp); //获得合并后的数值
			//判断是否可以移动，可以移动，则新出现一个数字
			//不能移动，但按键了，也不会出现新数字
// 			if (!ifMove(tp))
// 			{
// 				j++; //向上移动
// 			}
			BOX[0][i] = p[0]; //把处理好的中间变量移回来
			BOX[1][i] = p[1];
			BOX[2][i] = p[2];
			BOX[3][i] = p[3];
		}
		return j != 4; //当j不超过4的时候，可以执行up操作
	case 80: //down
		j = 0; 
		for (i = 0; i < 4; i++)
		{
			tp[0] = BOX[3][i];
			tp[1] = BOX[2][i];
			tp[2] = BOX[1][i];
			tp[3] = BOX[0][i];
			p = add(tp);
// 			if (!ifMove(tp))
// 			{
// 				j++;
// 			}
			BOX[3][i] = p[0];
			BOX[2][i] = p[1];
			BOX[1][i] = p[2];
			BOX[0][i] = p[3];
		}
		return j != 4;
	case 75: //left
		j = 0;
		for (i = 0; i < 4; i++)
		{
			tp[0] = BOX[i][0];
			tp[1] = BOX[i][1];
			tp[2] = BOX[i][2];
			tp[3] = BOX[i][3];
			p = add(tp);
// 			if (!ifMove(tp))
// 			{
// 				j++;
// 			}
			BOX[i][0] = p[0];
			BOX[i][1] = p[1];
			BOX[i][2] = p[2];
			BOX[i][3] = p[3];
		}
		return j != 4;
	case 77: //right
		j = 0;
		for (i = 0; i < 4; i++)
		{
			tp[0] = BOX[i][3];
			tp[1] = BOX[i][2];
			tp[2] = BOX[i][1];
			tp[3] = BOX[i][0];
			p = add(tp);
// 			if (!ifMove(tp))
// 			{
// 				j++;
// 			}
			BOX[i][3] = p[0];
			BOX[i][2] = p[1];
			BOX[i][1] = p[2];
			BOX[i][0] = p[3];
		}
		return j != 4;
	case 27: //按Esc
		setPos(20, 23);
		color(12);
		printf("确定退出游戏么？(y/n)");
		char c = _getch();  //获得键盘输入
		if (c == 'y' || c == 'Y') //如果输入的是小写或大写的y
		{
			exit(0);  //退出游戏
		}
		if (c == 'n' || c == 'N')  //如果输入的是大写或小写的n
		{
			setPos(20, 23);
			printf("                        ");
		}
		break;
	default: return 0;
	}
	return 1;
}


//退出
void close()
{
	exit(0);
}

//开始游戏
void gamePlay()
{
	system("cls");  //清屏
	int i = 0, j = 0;
	Time = time(NULL); //取当前时间为开始时间
	drawTheGameBox(); //绘制游戏界面
	int a, b; //BOX[][]数组的横纵坐标
	srand((unsigned)time(NULL)); //设置随机数种子
	do 
	{
		//获得4*4棋盘中的随机位置
		a = rand() % 4;
		b = rand() % 4;
	} while (BOX[a][b] != 0); //一直到棋盘中没有空格

	if (rand() % 4 == 0)  //2或4随机出现在空格处
	{
		BOX[a][b] = 4;
	}
	else
	{
		BOX[a][b] = 2;
	}

	for (i = 0; i < 4; i++)  //遍历整个网格
	{
		for (j = 0; j < 4; j++)
		{
			if (BOX[i][j] == 0) //如果网格中有空位
			{
				continue;
			}
			setPos(15 + j * 10 + 5, 2 + i * 5 + 3);
			int c = BOX[i][j]; //获得棋盘上BOX[i][j]上的数字
			printf("%d", c);
		}
	}

	while (1)
	{
		while (_kbhit()) //检查当前是否有键盘输入，有返回1，否则返回0
		{
			//如果按下的按键不是在keyboardControl()方法中定义的，会没有反应
			//一直到按下定义的按键为止
			if (!keyboardControl(_getch()))
			{
				continue;
			}
			drawTheGameBox(); //绘制棋盘
			for (i = 0; i < 4; i++)
			{//遍历棋盘
				for (j = 0; j < 4; j++)
				{
					if (BOX[i][j] == 0) //如果棋盘中有空位，则可一直进行按键
					{
						continue;
					}
					setPos(15 + j * 10 + 5, 2 + i * 5 + 3); //合并后的数出现的位置
					int c = BOX[i][j];
					textColor(c);
					printf("%d", c);
				}
			}
			do 
			{ //获得随机位置
				a = rand() % 4;
				b = rand() % 4;
			} while (BOX[a][b] != 0);

			if (rand() % 4 == 0) //2或4随机出现在空格处
			{
				BOX[a][b] = 4; 
			}
			else
			{
				BOX[a][b] = 2;
			}
			step++;  //进行计步
			setPos(15 + b * 10 + 5, 2 + a * 5 + 3);
			int c = BOX[a][b];
			textColor(c);
			printf("%d", c);
		}
		//只要gamefaile()或者gamewin()任意一个函数返回1，
		//也就是成功或是失败都会出现下面的内容
		if (gameFaile() + gameWin() != 0)
		{
			int n;
			setPos(20, 20);
			color(12);
			printf("我要重新玩一局-------1");
			setPos(45, 20);
			printf("不玩了，退出吧-------2\n");
			setPos(43, 21);
			color(11);
			scanf_s("%d", &n);
			switch (n)
			{
			case 1:
				replay(); //重新游戏函数
				break;
			case 2:
				close();
				break;
			}
		}
	}
}

//重新开始游戏
void replay()
{
	system("cls");
	score = 0, step = 0; //分数、步数归零
	memset(BOX, 0, 16 * sizeof(int)); //初始化BOX数组
	gamePlay();  //开始游戏
}

//设置标题
void title()
{
	color(11);          //浅淡绿色
	system("title 2048 Online");
	setPos(19, 2);
	printf("■■■   ■■■■   ■  ■     ■■■■"); //输出2048字符画
	setPos(19, 3);
	printf("    ■   ■    ■   ■  ■     ■    ■");
	setPos(19, 4);
	printf("■■■   ■    ■   ■  ■     ■■■■");
	setPos(19, 5);
	printf("■       ■    ■   ■■■■   ■    ■");
	setPos(19, 6);
	printf("■■■   ■■■■       ■     ■■■■");
}


//菜单选项
void choice()
{
	int n;
	int i, j = 1;
	setPos(32, 8);
	color(13);
	printf("挑 战 2 0 4 8");
	color(14);  //黄色边框
	for (i = 9; i <= 20; i++)
	{
		for (j = 15; j <= 60; j++)
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
	printf("2.游戏规则");
	setPos(25, 16);
	printf("3.按键说明");
	setPos(40, 16);
	printf("4.退出");
	setPos(21, 22);
	color(5);
	printf("请选择[1 2 3 4]:[ ]\b\b");
	scanf_s("%d", &n); //输入选项
	switch (n)
	{
	case 1:
		gamePlay(); //游戏开始函数
		break;
	case 2:
	
		break;
	case 3:
		
		break;
	case 4:
		close();  //关闭游戏函数
		break; 
	}

}



