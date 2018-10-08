#include "Astar.h"

// 构造函数
CAstar::CAstar()
{
	m_bOpenMap[MAPHEIGHT][MAPWIDTH] = { false };
	m_pCloseMap[MAPHEIGHT][MAPWIDTH] = { NULL };
	m_bValidMap[MAPHEIGHT][MAPWIDTH] = { true };
}

// 析构函数
CAstar::~CAstar()
{
	Clear();
}

// 清除
void CAstar::Clear()
{	// 遍历已经检测过的点
	for (size_t i = 0; i < m_vecClose.size(); i++) {
		if (m_vecClose[i]) {
			delete[] m_vecClose[i];
			m_vecClose[i] = NULL;
		}
	}
	//m_vecClose.clear();
	m_vecClose.swap(vector<STARNODE*>());

	// 遍历待检测的点
	for (size_t i = 0; i < m_vecOpen.size(); i++) {
		if (m_vecOpen[i]) {
			delete[] m_vecOpen[i];
			m_vecOpen[i] = NULL;
		}
	}
	//m_vecOpen.clear();
	m_vecOpen.swap(vector<STARNODE*>());
}

// 获取移动方向
int  CAstar::getMoveDir(POS startPos)
{
	// 获取第一次移动的点
	int nToX = 0;
	int nToY = 0;
	int nFromX;
	int nFromY;
	bool hasFind = false;
	STARNODE* pNode = m_vecClose[m_vecClose.size() - 1];
	int nMaxLoopTimes = m_vecClose.size() + 1;
	int nCurrentLoop = 0; // 防止死循环
	while (!hasFind) {
		nToX = pNode->nPosX;
		nToY = pNode->nPosY;
		nFromX = pNode->nFromX;
		nFromY = pNode->nFromY;
		//delete pNode;
		// 上一个点
		pNode = m_pCloseMap[nFromY][nFromX];

		if (pNode->nFromX == 0 && pNode->nFromY == 0 && pNode->nG == 0) { // 找到了开始点
			hasFind = true;
		}
		nCurrentLoop++;
		if (nCurrentLoop > nMaxLoopTimes) {
			//printf("error_Astar\n");
			return 0;
		}
	}
	// 找到了第一步要移动的位置（nToX, nToY）
	if (startPos.posX == nToX) { // 上下移动
		if (nToY - startPos.posY == -1) { // 上
			return 0;
		}
		else {
			return 2;
		}
	}
	else if (startPos.posY == nToY) {
		if (nToX - startPos.posX == -1) {// 左
			return 3;
		}
		else {
			return 1;
		}
	}
	else {
		printf("debug: 错误的Astart方向\n");
		return -1;
	}

}

// 搜寻路径
bool  CAstar::searchPath(CMaps& maps, POS startPoint, POS endPoint)
{
	// 初始化信息
	initData(maps, startPoint);
	// 初始化起点、终点2个节点
	//pEndNode
	STARNODE* pEndNode = new STARNODE;
	pEndNode->nPosX = endPoint.posX;
	pEndNode->nPosY = endPoint.posY;

	//pStartNode
	STARNODE*   pStartNode = new STARNODE;
	pStartNode->nG = 0;
	pStartNode->nPosX = startPoint.posX;
	pStartNode->nPosY = startPoint.posY;
	pStartNode->nFromX = 0;
	pStartNode->nFromY = 0;
	pStartNode->setH_F(pEndNode);

	// 加入open表
	addToOpenTable(pStartNode);
	// 循环扩散
	while (1) {
		// 判断open表是否为空
		if (m_vecOpen.empty()) {
			return false;
		}
		// open表中取出一个最小F的点（排序后的第一个点）
		STARNODE* pCenterNode = m_vecOpen[0];
		// 扩散取出的点
		STARNODE* pNearNode[4]; // 上右下左
		for (int i = 0; i < 4; i++) {
			pNearNode[i] = new STARNODE{};
		}
		int nCenterPosX = pCenterNode->nPosX;
		int nCenterPosY = pCenterNode->nPosY;
		// 设置四个点信息
		pNearNode[0]->nPosX = nCenterPosX;
		pNearNode[0]->nPosY = nCenterPosY - 1;
		pNearNode[1]->nPosX = nCenterPosX + 1;
		pNearNode[1]->nPosY = nCenterPosY;
		pNearNode[2]->nPosX = nCenterPosX;
		pNearNode[2]->nPosY = nCenterPosY + 1;
		pNearNode[3]->nPosX = nCenterPosX - 1;
		pNearNode[3]->nPosY = nCenterPosY;

		for (int i = 0; i < 4; i++) {
			pNearNode[i]->nFromX = nCenterPosX;
			pNearNode[i]->nFromY = nCenterPosY;
			pNearNode[i]->nG = pCenterNode->nG + 1;
			pNearNode[i]->setH_F(pEndNode);
		}
		// 扩散完成： 删除open表扩散点，插入close表
		m_vecClose.push_back(m_vecOpen[0]);
		m_pCloseMap[nCenterPosY][nCenterPosX] = m_vecOpen[0];
		m_vecOpen.erase(m_vecOpen.begin());
		// 判断四个点是否有效
		for (int i = 0; i < 4; i++) {
			int x = pNearNode[i]->nPosX;
			int y = pNearNode[i]->nPosY;
			// 判断是不是终点
			if (x == pEndNode->nPosX && y == pEndNode->nPosY) {
				m_vecClose.push_back(pNearNode[i]);
				m_pCloseMap[x][y] = pNearNode[i];
				// 释放
				for (int j = i + 1; j < 4; j++) {
					delete[]  pNearNode[j];
				}
				return true;
			}
			// 是否是地图上的有效点
			if (!m_bValidMap[y][x]) {
				delete pNearNode[i];
				continue;
			}
			// 是否在Open表 或close表中
			if (m_bOpenMap[y][x] || m_pCloseMap[y][x] != NULL) {
				delete pNearNode[i];
				continue;
			}
			// 合格了，添加到open表中
			addToOpenTable(pNearNode[i]);
		}

	}
}

// 保存待检测的点
void CAstar::addToOpenTable(STARNODE * pNewNode)
{
	int x = pNewNode->nPosX;
	int y = pNewNode->nPosY;
	m_bOpenMap[y][x] = true;

	if (m_vecOpen.size() == 0) {
		m_vecOpen.push_back(pNewNode);
		return;
	}
	for (unsigned int i = 0; i < m_vecOpen.size(); i++) {
		if (pNewNode->nF < m_vecOpen[i]->nF) {
			m_vecOpen.insert(m_vecOpen.begin() + i, pNewNode);
			return;
		}
		else if (i == m_vecOpen.size() - 1) {
			m_vecOpen.push_back(pNewNode);
			return;
		}
	}
}

// 初始化路径
void CAstar::initData(CMaps& maps, POS startPoint)
{
	//m_vecClose.clear();
	//m_vecOpen.clear();
	Clear();
	// 初始化
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			m_bOpenMap[row][col] = false;
			m_pCloseMap[row][col] = NULL;
		}
	}
	updateInvalidPoint(maps, startPoint);
}

// 更新失效的点
void CAstar::updateInvalidPoint(CMaps& maps, POS startPoint)
{
	// 设置更新时间
	static clock_t last_time = 0;
	clock_t now_time = clock();
	if (now_time - last_time < 5000) {
		//return;
	}
	last_time = now_time;
	// init
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			m_bValidMap[row][col] = true;
		}
	}
	// update
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			if (col == 0 || col == MAPWIDTH - 1 || row == 0 || row == MAPHEIGHT - 1) {
				m_bValidMap[row][col] = false; // 为下面的判断做铺垫，防止row-1越界
			}
			else if (maps.m_nMap[row][col] == MAP_STONE || maps.m_nMap[row][col] == MAP_BRICK || maps.m_nMap[row][col] == MAP_RIVER ||
					(maps.m_pTankMap[row][col] != NULL &&  maps.m_pTankMap[row][col]->isNPC &&
					((abs(maps.m_pTankMap[row][col]->posX - startPoint.posX) >= 1 && abs(maps.m_pTankMap[row][col]->posX - startPoint.posX) < 5) ||
					(abs(maps.m_pTankMap[row][col]->posY - startPoint.posY) >= 1 && abs(maps.m_pTankMap[row][col]->posY - startPoint.posY) < 5)))) {
					// 四周的点都是无效点
					m_bValidMap[row - 1][col - 1] = false;
					m_bValidMap[row - 1][col] = false;
					m_bValidMap[row - 1][col + 1] = false;
					m_bValidMap[row][col - 1] = false;
					m_bValidMap[row][col] = false;
					m_bValidMap[row][col + 1] = false;
					m_bValidMap[row + 1][col - 1] = false;
					m_bValidMap[row + 1][col] = false;
					m_bValidMap[row + 1][col + 1] = false;
			}
			else if (row == MAPHEIGHT - 3 || row == MAPHEIGHT - 4) { // 老巢位置
				m_bValidMap[row][col] = true;
			}
			else if (col == 0 || col == 1 || col == MAPWIDTH - 1 || col == MAPWIDTH - 2 ||
				row == 0 || row == 1 || row == MAPHEIGHT - 1 || row == MAPHEIGHT - 2) {
				m_bValidMap[row][col] = false;
			}
		}
	}

}


