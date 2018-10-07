#pragma once

/*******函  数  声  明 *******/

void createMap();           //绘制地图
void initsnake();           //初始化蛇身，画蛇身
void createfood();          //创建并随机出现食物										 
void keyboardControl();     //控制键盘按键
void speedup();				//加速
void speeddown();			//减速
void snakemove();           //控制蛇前进方向
int  biteself();            //判断是否咬到了自己
void cantcrosswall();       //设置蛇撞墙的情况
void endgame();             //游戏结束
void File_out();            //在文件中读取最高分
void File_in();            	//储存最高分进文件						
// void OutputPos(int nX, int nY);// 输出调试坐标信息
// void WriteChar(int x, int y, const char *szInfo, int co);//实现一个能在屏幕任何位置输出字符串的函数		
// void MouseEventProc(MOUSE_EVENT_RECORD mer);//鼠标事件处理函数













