#pragma once

/*******宏  定  义*******/
#define DIR_UP     1
#define DIR_DOWN   2
#define DIR_LEFT   3 
#define DIR_RIGHT  4      //蛇的方向，Up：上 ；Down：下；Left:左 Right：右


extern int score, add;	      //总得分与每次吃食物得分
extern int HighScore;		  //最高分
extern int status, sleeptime; //蛇前进状态，每次运行的时间间隔
extern int endgamestatus;     //游戏结束的情况，1：撞到墙；2：咬到自己；3：主动退出游戏
	 
