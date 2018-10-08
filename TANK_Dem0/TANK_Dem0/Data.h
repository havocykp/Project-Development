#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <vector>
using namespace std;


#define MAPWIDTH  60
#define MAPHEIGHT  40

// 地图内的静态物体
#define MAP_SPACE 0
#define MAP_STONE 2
#define MAP_BRICK 4
#define MAP_GRASS 8
#define MAP_ICE   16
#define MAP_TREE  32
#define MAP_RIVER 64
#define MAP_HEART  128

// 坦克方向（对应坦克形状数组下标，不能修改）
#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN  2
#define DIR_LEFT 3

// 控制台颜色
#define	COLOR_BLACK   0
#define	COLOR_PINK   13
#define	COLOR_RED   12 // 鲜红
#define	COLOR_RED_LIGHT   4 // 暗红
#define	COLOR_YELLOW   14
#define	COLOR_YELLOW_LIGHT   6
#define	COLOR_BLUE   9
#define	COLOR_BLUE_LIGHT   1
#define	COLOR_GREEN   10
#define	COLOR_GREEN_LIGHT   2
#define	COLOR_WHITE   15
#define	COLOR_GRAY   8  //灰色


#define TANK_SHAPE1   1 // "◆"
#define TANK_SHAPE2   2 // "●"
#define TANK_SHAPE3   3 // "◎"
#define TANK_SHAPE4   4 // "□"
#define TANK_SHAPE5   5 // "■"

// 坐标结构体
typedef struct POS {
	int posX;
	int posY;
} POS;

// 子弹
typedef struct BULLET {
	int posX;
	int posY;
	int dir;
	bool isNPC;
	int tankId;
	bool isValid;
	int   power; // 攻击力
	clock_t last_fly_time;
	int color; 
} BULLET;

// 坦克结构体
typedef struct TANK {
	int posX;
	int posY;
	int initPosX;
	int initPosY;
	bool isNPC;
	int dir;
	int color;

	int maxHP;
	int curHP;

	int maxSpeed;
	int curSpeed;
	int oldSpeed;

	int nKill;
	int nDie;
	int nlife;

	int nlevel;
	int nShape;
	bool ready;

	clock_t last_move_time;
	clock_t last_shoot_time;

	bool isAlive;
} TANK;

// 星节点
typedef struct STARNODE {
	int nG; // 移动损耗
	int nH; // 最短距离
	int nF; // nG + nH
	int nPosX;
	int nPosY;
	int nFromX;
	int nFromY;
	void setH_F(STARNODE* star)
	{
		nH = abs(nPosX - star->nPosX) + abs(nPosY - star->nPosY);
		nF = nH + nG;
	}
}STARNODE;

