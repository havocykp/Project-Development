#pragma once
#include <sstream>
#include "Data.h"

class CMaps
{
public:
	// 初始化外墙
	void initOuterWall();
	// 老巢
	void initHeart();
	// 初始化地图中的障碍物
	void initStaticMapData();
	// 自定义地图
	void customMapData();
	// 显示自定义地图需要的物体
	void showNeedStaticObj();
	// 打印地图静态物体
	void drawMap();
	// 恢复受损的植物
	void recoverDamagedPlant();
	// 读取地图文件
	void readStaticMapFile(int fileNum);
	// 画静态物体点： 依据m_nMap
	void reDrawMapPoint(int row, int col);
	// 指定位置打印
	void static printChar(int posX, int posY, const char* pszCha, WORD wArr);
public:
	CMaps();
	~CMaps();
public:
	int m_nMap[MAPHEIGHT][MAPWIDTH];
	TANK * m_pTankMap[MAPHEIGHT][MAPWIDTH];
	BULLET *  m_pBulletMap[MAPHEIGHT][MAPWIDTH];
	vector<POS> m_vecDamagedPlant;
	bool m_isHeartBroken;
};


