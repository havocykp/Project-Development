#pragma once
#include "Data.h"
#include "Maps.h"
#include "Bgm.h"

class CTank;

class CBullet
{
public:
	CBullet(CMaps* pMaps);
	~CBullet();
public:
	// 发射子弹
	void shootBullet(vector<TANK *>& m_vecTank, int nTankIndex);
	// 测试子弹是否可以发射了
	void canShoot(vector<TANK*>&m_vecTank);
	// 子弹飞行 
	bool runBullet();
	// 绘制子弹
	void drawBullet(bool isShow);
	// 移除无效的子弹
	void removeInvalidBullet();
public:
	vector<BULLET *> m_vecBullet;
	CMaps * m_pMaps;
};


