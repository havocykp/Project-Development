#pragma once
#include "Data.h"
#include "Maps.h"
class CAstar
{
public:
	CAstar();
	~CAstar();
public:
	// 搜寻路径
	bool searchPath(CMaps& maps, POS startPoint, POS endPoint);
	// 获取移动方向
	int  getMoveDir(POS startPos);
private:
	// 初始化路径
	void initData(CMaps& maps, POS startPoint);
	// 保存需要检测的点
	void addToOpenTable(STARNODE * pNode);
	// 更新失效的点
	void updateInvalidPoint(CMaps& maps, POS startPoint);
	// 清除
	void Clear();
private:
	vector<STARNODE*> m_vecOpen;
	vector<STARNODE*> m_vecClose;
	bool m_bOpenMap[MAPHEIGHT][MAPWIDTH];
	STARNODE*  m_pCloseMap[MAPHEIGHT][MAPWIDTH];;
	bool m_bValidMap[MAPHEIGHT][MAPWIDTH];
};

