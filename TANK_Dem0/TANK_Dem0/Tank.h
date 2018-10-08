#pragma once
#include "Data.h"
#include "Maps.h"
#include "Bullet.h"
#include "Astar.h"
class CTank
{
public:
	CTank(CMaps* pMaps);
	~CTank();
public:
	// 初始化Npc坦克
	void initNpcTank(int count, int nLevel = 0);
	// 初始化玩家坦克
	void initPlayerTank(int count, int nLevel = 0);
	// 自动运行Npc坦克
	void autoRunNpcTank(CBullet& bullets);
	// 显示坦克
	void drawTank(int index, bool isShow);
	void initDrawAllTank(bool isShow);
	// 移动坦克, 包含显示和隐藏
	bool moveTank(int nDir, int index);
	// 判断血值和存活： 所有的坦克
	int judgeAlive();
	// 显示坦克的游戏信息
	void showTankInfo();
	// 获取安全的出生点
	void getSafedPoint(int tankIndex, int& posX, int&posY);
	// 回到出生点
	void helpPlayer();
private:
	// 初始化坦克形状
	void initTankShapeModel();
public:
	vector<TANK *>  m_vecTank;
	int m_tankShape[4][3][3]; // TankShape[4][3][3]
	CMaps * m_pMaps;
	int m_nScore;
	int m_nLevel;
};

