#include "Tank.h"

// 构造函数
CTank::CTank(CMaps* pMaps)
{
	initTankShapeModel();
	m_pMaps = pMaps;
}

// 析构函数
CTank::~CTank()
{
}

// 初始化坦克形状
void CTank::initTankShapeModel()
{
	int tankShape[4][3][3] = {
		{// 向上
			{ 0, 1, 0 },
			{ 1, 1, 1 },
			{ 1, 0, 1 }
		},
		{// 向右
			{ 1, 1, 0 },
			{ 0, 1, 1 },
			{ 1, 1, 0 }
		},
		{ // 向下
			{ 1, 0, 1 },
			{ 1, 1, 1 },
			{ 0, 1, 0 }
		},
		{ // 向左
			{ 0, 1, 1 },
			{ 1, 1, 0 },
			{ 0, 1, 1 }
		}
	};
	// 循环遍历打印
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				m_tankShape[i][j][k] = tankShape[i][j][k];
			}
		}
	}
}

// 初始化NPC坦克
void CTank::initNpcTank(int Count, int nLevel)
{
	m_nLevel = nLevel;
	srand((unsigned int)time(NULL));
	Count = Count > 18 ? 18 : Count;
	for (int nNum = 0; nNum < Count; nNum++) {
		TANK*  pTank = (TANK*)malloc(sizeof(TANK));
		pTank->isNPC = true;
		pTank->color = COLOR_YELLOW;
		pTank->dir = DIR_DOWN;
		pTank->maxHP = 60 + nNum * 20 + nLevel;
		pTank->curHP = pTank->maxHP;
		pTank->maxSpeed = 100 - nNum * 10 + nLevel;
		pTank->curSpeed = pTank->maxSpeed;
		pTank->oldSpeed = 0;
		pTank->nKill = 0;
		pTank->nDie = 0;
		pTank->nlife = 0;
		pTank->nlevel = 1;
		pTank->last_move_time = 0;
		pTank->last_shoot_time = 0;
		pTank->isAlive = true;
		pTank->nShape = nNum % 4 + 1;
		// 坐标
		int X;
		int Y = 2;
		if (nNum % 2 == 0) { // 布置在左边
			X = 2 + (nNum / 2) * 3;
		}
		else { // 布置在右边
			X = MAPWIDTH - 3 - ((nNum - 1) / 2) * 3;
		}
		pTank->posX = X;
		pTank->posY = Y;
		pTank->initPosX = X;
		pTank->initPosY = Y;
		m_vecTank.push_back(pTank);
		drawTank(m_vecTank.size() - 1, true);
	}
}

// NPC自动坦克
void CTank::autoRunNpcTank(CBullet& bullets)
{
	srand((unsigned)time(NULL));
	POS endHeart; // 目标老巢
	endHeart.posX = MAPWIDTH / 2 - 1;
	endHeart.posY = MAPHEIGHT - 3;
	// 坦克1号
	POS player01;
	player01.posX = m_vecTank[0]->posX;
	player01.posY = m_vecTank[0]->posY;
	// 坦克2号
	POS player02;
	bool mayHasPlayer02 = false;
	if (m_vecTank.size() > 1) {
		player02.posX = m_vecTank[1]->posX;
		player02.posY = m_vecTank[1]->posY;
		mayHasPlayer02 = true;
	}
	// 寻路目标
	POS targetPos;

	bool bToHeart = false;
	bool bToPlayer01 = false;
	bool bToPlayer02 = false;
	static CAstar Astar;
	for (size_t index = 0; index < m_vecTank.size(); index++) {
		if (m_vecTank[index]->isNPC && m_vecTank[index]->isAlive) {

			POS start;
			start.posX = m_vecTank[index]->posX;
			start.posY = m_vecTank[index]->posY;
			if (bToHeart == false) {
				targetPos = endHeart;
				bToHeart = true;
			}
			else if (m_vecTank[0]->isAlive && bToPlayer01 == false) {
				targetPos = player01;
				bToPlayer01 = true;
			}
			else if (mayHasPlayer02 && !m_vecTank[1]->isNPC && m_vecTank[1]->isAlive) {
				targetPos = player02;
				bToPlayer02 = true;
			}
			else {
				targetPos = player01;
			}

			int nDir = -1;
			// 测试
			if (true) { // true--自动寻路
				targetPos = player01;
				if (Astar.searchPath(*m_pMaps, start, targetPos)) {
					nDir = Astar.getMoveDir(start);
				}
				if (nDir == -1) {
					continue;
				}
			}
			if (nDir == -1) {
				nDir = rand() % 4;
			}


			moveTank(nDir, index); // 移动
			bullets.shootBullet(m_vecTank, index); // 射击
		}
	}


}

// 初始化玩家坦克
void CTank::initPlayerTank(int Count, int nLevel)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < Count; i++) {
		TANK*  pTank = (TANK*)malloc(sizeof(TANK));
		pTank->isNPC = false;
		pTank->posX = MAPWIDTH / 2 - 4 + 8 * i;
		pTank->posY = MAPHEIGHT - 3;
		pTank->initPosX = pTank->posX;
		pTank->initPosY = pTank->posY;
		pTank->color = COLOR_GREEN;
		pTank->dir = DIR_UP;
		pTank->maxHP = 100 + nLevel;
		pTank->curHP = pTank->maxHP;
		pTank->maxSpeed = 170 + nLevel * 10;
		pTank->curSpeed = pTank->maxSpeed;
		pTank->oldSpeed = 0;
		pTank->nKill = 0;
		pTank->nDie = 0;
		pTank->nlife = 3;
		pTank->nlevel = 1;
		pTank->last_move_time = 0;
		pTank->last_shoot_time = 0;
		pTank->isAlive = true;
		pTank->nShape = TANK_SHAPE5;

		m_vecTank.push_back(pTank);
		drawTank(m_vecTank.size() - 1, true);
	}

}

// 画坦克
void CTank::drawTank(int index, bool isShow)
{
	srand((unsigned)time(NULL));
	// 初始化速度
	m_vecTank[index]->curSpeed = m_vecTank[index]->maxSpeed;
	int posX = m_vecTank[index]->posX;
	int posY = m_vecTank[index]->posY;
	int dirNum = m_vecTank[index]->dir;
	for (int row = posY - 1; row <= posY + 1; row++) {
		for (int col = posX - 1; col <= posX + 1; col++) {
			if (m_tankShape[dirNum][row - posY + 1][col - posX + 1] == 1) {
				if (m_pMaps->m_nMap[row][col] == MAP_TREE) {
					// 树林要隐藏坦克： 不显示也不擦除
				}
				else if (isShow) {
					// 按类型显示坦克
					const char * shape = NULL;
					switch (m_vecTank[index]->nShape) {
					case TANK_SHAPE1:
						shape = "◆";
						break;
					case TANK_SHAPE2:
						shape = "●";
						break;
					case TANK_SHAPE3:
						shape = "◎";
						break;
					case TANK_SHAPE4:
						shape = "□";
						break;
					case TANK_SHAPE5:
						shape = "■";
						break;
					}
					m_pMaps->printChar(col, row, shape, m_vecTank[index]->color);
					m_pMaps->m_pTankMap[row][col] = m_vecTank[index]; // 同步坦克地图
				    // 如果显示位置有植物
					if (m_pMaps->m_nMap[row][col] == MAP_GRASS || m_pMaps->m_nMap[row][col] == MAP_ICE) {//植物被破坏
						m_pMaps->m_vecDamagedPlant.push_back({ col, row });   // 同步受损植物地图

						if (m_pMaps->m_nMap[row][col] == MAP_GRASS) {// 减速
							m_vecTank[index]->curSpeed = int(m_vecTank[index]->maxSpeed / (rand() % 4 + 1));
						}
						else {// 加速
							m_vecTank[index]->curSpeed = int(m_vecTank[index]->maxSpeed + m_vecTank[index]->maxSpeed / (rand() % 4 + 1));
						}
					}
					else if (m_pMaps->m_nMap[row][col] == MAP_HEART) {// 检测老巢是否被碾压
						m_pMaps->m_isHeartBroken = true;
					}


				}
				else {
					m_pMaps->printChar(col, row, "  ", COLOR_BLACK);
					m_pMaps->m_pTankMap[row][col] = NULL;
					if (m_pMaps->m_nMap[row][col] == MAP_GRASS || m_pMaps->m_nMap[row][col] == MAP_ICE) {//植物被破坏
						m_pMaps->m_vecDamagedPlant.push_back({ col, row });  // 同步受损植物地图
					}
				}
			}
		}
	}

}

// 初始化全部坦克
void CTank::initDrawAllTank(bool isShow)
{
	for (size_t i = 0; i < m_vecTank.size(); i++) {
		drawTank(i, true);
	}
}

// 坦克移动
bool CTank::moveTank(int nDirNum, int index = 0)
{
	if (m_vecTank[index]->isAlive == false) {
		return false;
	}
	// 初始化位置、方向
	int nDestPosX = m_vecTank[index]->posX;
	int nDestPosY = m_vecTank[index]->posY;
	int nDestDir = m_vecTank[index]->dir;
	// 判断移动操作，否则坐标不变
	int blockNumSum = MAP_STONE + MAP_BRICK + MAP_RIVER;
	int changeValue = m_vecTank[index]->dir - nDirNum;
	if (changeValue == 0) { //前进，修改坐标,方向不变
		// 速度设置：只有前进限制速度
		clock_t start_time = m_vecTank[index]->last_move_time;
		clock_t end_time = clock();
		if (end_time - start_time < (200 - m_vecTank[index]->curSpeed) * 4) {
			Sleep(20);
			//printf("(%d,%d)\n", end_time - start_time, (200 - m_vecTank[index]->curSpeed) * 4);
			return false;
		}
		m_vecTank[index]->last_move_time = end_time;
		//开始判断
		switch (m_vecTank[index]->dir) {
		case DIR_UP:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 2][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 1] != NULL || m_pMaps->m_pTankMap[nDestPosY - 2][nDestPosX] != NULL || m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 1] != NULL) {
				return false;//左中右--目的位置有墙
			}
			nDestPosY--;
			break;
		case DIR_RIGHT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX + 2] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 1] != NULL || m_pMaps->m_pTankMap[nDestPosY][nDestPosX + 2] != NULL || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 1] != NULL) {
				return false;//上中下--目的位置有墙
			}
			nDestPosX++;
			break;
		case DIR_DOWN:
			if ((m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 2][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 2][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//左中右--目的位置有墙
			}
			nDestPosY++;
			break;
		case DIR_LEFT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX - 2] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX - 2] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0) {
				return false;//上中下-目的位置有墙
			}
			nDestPosX--;
			break;
		default:
			//printf("坦克前进方向参数dirNum错误（0123）： moveTank\n");
			return false;
		}
	}
	else if (changeValue == -1 || changeValue == -3 || changeValue == 1 || changeValue == 3) { //左右移动: 只改变方向（左右肩和尾部不能有物体）
		switch (m_vecTank[index]->dir) {
		case DIR_UP:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 1] != 0) {
				return false;//左下右
			}
			break;
		case DIR_RIGHT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 1] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//上左下
			}
			break;
		case DIR_DOWN:
			if ((m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 1][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//左上右
			}
			break;
		case DIR_LEFT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX + 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX + 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0) {
				return false;//上右下
			}
			break;
		default:
			//printf("坦克向左右方向移动参数dirNum错误（0123）： moveTank\n");
			return false;
		}
		nDestDir = nDirNum;
	}
	else if (changeValue == 2 || changeValue == -2) { // 反向移动: 只改变坐标，方向不变（尾部三个方向不能有物体）
		 // 尝试两次顺时针转
		int nTryDir = m_vecTank[index]->dir + 1;
		nTryDir = nTryDir <= 3 ? nTryDir : 0;
		if (moveTank(nTryDir, index)) { // 移动成功， 再顺时针移动一次
			nTryDir++;
			nTryDir = nTryDir <= 3 ? nTryDir : 0;
			if (moveTank(nTryDir, index)) { // 二次成功
				return true;
			}
			else {// 逆时针，还原方向
				nTryDir--;
				nTryDir = nTryDir >= 0 ? nTryDir : 0;
				moveTank(nTryDir, index);
				// 只能倒退了
			}
		}

		// 开始倒车的判断
		switch (m_vecTank[index]->dir) {
		case DIR_UP:
			if ((m_pMaps->m_nMap[nDestPosY + 2][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 2][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY + 2][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY + 2][nDestPosX + 1] != 0) {
				return false;//左中右
			}
			nDestPosY++;
			break;
		case DIR_RIGHT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 2] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 2] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 2] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 2] != 0) {
				return false;//上中下
			}
			nDestPosX--;
			break;
		case DIR_DOWN:
			if ((m_pMaps->m_nMap[nDestPosY - 2][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 1][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 2][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 2][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY - 2][nDestPosX + 1] != 0) {
				return false;//左中右
			}
			nDestPosY--;
			break;
		case DIR_LEFT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 2] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX + 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 2] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 2] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX + 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 2] != 0) {
				return false;//上中下
			}
			nDestPosX++;
			break;
		default:
			//printf("坦克向右方向参数dirNum错误（0123）： moveTank\n");
			return false;
		}
	}
	else {
		//printf("坦克移动方向有误：tank.cpp/moveTank\n");
	}

	//允许移动
	drawTank(index, false); //擦除
    //修改属性
	m_vecTank[index]->posX = nDestPosX;
	m_vecTank[index]->posY = nDestPosY;
	m_vecTank[index]->dir = nDestDir;
	//绘制
	drawTank(index, true);
	return true;
}

// 判断是否存活
int  CTank::judgeAlive()
{
	int playerNum = 0;
	int NpcNum = 0;
	for (size_t i = 0; i < m_vecTank.size(); i++) {
		if (m_vecTank[i]->curHP > 0 || m_vecTank[i]->isAlive == false) { //不用判断:有血或者已死
			if (m_vecTank[i]->isNPC && m_vecTank[i]->curHP > 0) {
				NpcNum++;
			}
			else if (m_vecTank[i]->curHP > 0) {
				playerNum++;
			}
			continue;
		}
		if (m_vecTank[i]->isNPC) {
			m_nScore += m_vecTank[i]->maxHP + m_nLevel; //计算分数
		}
		if (m_vecTank[i]->nlife >= 1) { //用life换血
			m_vecTank[i]->curHP = m_vecTank[i]->maxHP;
			m_vecTank[i]->nlife = m_vecTank[i]->nlife - 1;
			m_vecTank[i]->nDie = m_vecTank[i]->nDie + 1;
			if (m_vecTank[i]->isNPC) {
				NpcNum++;
				//CBgm::play(BGM_KILLED_BY_PALYER);//bgm
			}
			else {
				playerNum++; //玩家死亡一次
				//CBgm::play(BGM_PLAYER_TANK_DIE);//bgm
			}
			drawTank(i, false);

			// 如果出生地没有没占领，则在出生地重生
			int initX;//出生点
			int initY;
			getSafedPoint(i, initX, initY);
			m_vecTank[i]->posX = initX;
			m_vecTank[i]->posY = initY;
			drawTank(i, true);

			continue;
		}
		// 坦克死亡
		m_vecTank[i]->isAlive = false;
		drawTank(i, false);
	}

	// 判断双方存活的坦克数量
	if (playerNum == 0) { //失败
		return -1;
	}
	else if (NpcNum == 0) { //胜利
		return 1;
	}
	else { // 继续
		return 0;
	}
}

// 显示坦克信息
void CTank::showTankInfo()
{
	stringstream stream;
	int posX = MAPWIDTH + 1;
	int posY;

	for (size_t index = 0; index < m_vecTank.size(); index++) {
		posY = 2 + 4 * index;
		int dirNum = 0;
		// 在右侧栏画坦克
		for (int row = posY - 1; row <= posY + 1; row++) {
			for (int col = posX - 1; col <= posX + 1; col++) {
				if (m_tankShape[dirNum][row - posY + 1][col - posX + 1] == 1) {
					const char * shape = NULL;
					switch (m_vecTank[index]->nShape) {
					case TANK_SHAPE1:
						shape = "◆";
						break;
					case TANK_SHAPE2:
						shape = "●";
						break;
					case TANK_SHAPE3:
						shape = "◎";
						break;
					case TANK_SHAPE4:
						shape = "□";
						break;
					case TANK_SHAPE5:
						shape = "■";
						break;
					}
					if (!m_vecTank[index]->isAlive) { //死亡
						m_pMaps->printChar(col, row, shape, COLOR_GRAY);
					}
					else {// m_vecTank[index]->color
						m_pMaps->printChar(col, row, shape, rand() % 0xF + 0x0);
					}
				}
			}
		}
		
		//显示坦克生命值
		stream.clear();
		stream << m_vecTank[index]->curHP;
		string strHp = stream.str();
		strHp.append(" ");
		for (int i = 0; i < (int)((m_vecTank[index]->curHP * 10000 / m_vecTank[index]->maxHP) * 5 / 10000); i++) {
			strHp.append(" ");
		}
		if (m_vecTank[index]->isAlive) {
			CMaps::printChar(posX + 1, posY - 1, " ", COLOR_GREEN);
			CMaps::printChar(posX + 1, posY - 1, (char *)strHp.c_str(), COLOR_GREEN);
		}
		else {
			CMaps::printChar(posX + 1, posY - 1, "              ", COLOR_GRAY);
		}

		//显示坦克生命数量
		stream.clear();
		stream.str(""); //防止内存累积
		int nLife = m_vecTank[index]->nlife;
		stream << nLife;
		string  strLife = stream.str();
		strLife.insert(0, " x ");
		if (m_vecTank[index]->isAlive) {
			CMaps::printChar(posX + 2, posY, "      ", m_vecTank[index]->color);
			CMaps::printChar(posX + 2, posY, (char *)strLife.c_str(), m_vecTank[index]->color);
		}
		else {
			CMaps::printChar(posX + 2, posY, "      ", COLOR_GRAY);
		}

		// 显示速度
		stream.clear();
		stream.str(""); //防止内存累积
		int nSpeed = m_vecTank[index]->curSpeed;
		stream << nSpeed;
		string  strSpeed = stream.str();
		strSpeed.insert(0, "  ");
		strSpeed.append(" Km/h");
		if (m_vecTank[index]->isAlive) {
			CMaps::printChar(posX + 2, posY + 1, " ", m_vecTank[index]->color);
			CMaps::printChar(posX + 2, posY + 1, (char *)strSpeed.c_str(), COLOR_WHITE);
		}
		else {
			CMaps::printChar(posX + 2, posY + 1, "          ", COLOR_GRAY);
		}


		stream.str(""); //防止内存累积
	}

	// 显示关卡
	stream.clear();
	stream.str(""); //防止内存累积
	int nLevel = m_nLevel;
	stream << nLevel;
	string  strLevel = stream.str();
	strLevel.insert(0, "当前关卡：  ");
	CMaps::printChar(posX + 2, posY + 5, "       ", COLOR_WHITE);
	CMaps::printChar(posX + 2, posY + 5, (char *)strLevel.c_str(), COLOR_WHITE);
	// 显示分数
	stream.clear();
	stream.str(""); //防止内存累积
	int nScore = m_nScore;
	stream << nScore;
	string  strScore = stream.str();
	strScore.insert(0, "当前分数：  ");
	CMaps::printChar(posX + 2, posY + 7, "       ", COLOR_WHITE);
	CMaps::printChar(posX + 2, posY + 7, (char *)strScore.c_str(), COLOR_WHITE);

	stream.str(""); //防止内存累积
}

// 出生地
void CTank::getSafedPoint(int tankIndex, int& posX, int&posY)
{
	posX = m_vecTank[tankIndex]->initPosX;
	posY = m_vecTank[tankIndex]->initPosY;
	while (!(m_pMaps->m_pTankMap[posY - 1][posX - 1] == NULL &&
			m_pMaps->m_pTankMap[posY - 1][posX] == NULL &&
			m_pMaps->m_pTankMap[posY - 1][posX + 1] == NULL &&
			m_pMaps->m_pTankMap[posY][posX - 1] == NULL &&
			m_pMaps->m_pTankMap[posY][posX] == NULL &&
			m_pMaps->m_pTankMap[posY][posX + 1] == NULL &&
			m_pMaps->m_pTankMap[posY + 1][posX - 1] == NULL &&
			m_pMaps->m_pTankMap[posY + 1][posX] == NULL &&
			m_pMaps->m_pTankMap[posY + 1][posX + 1] == NULL)) {
		if (tankIndex == 1 && !m_vecTank[1]->isNPC) {
			posX++;
			m_vecTank[tankIndex]->dir = 0;
		}
		else {
			posX--;
		}

		if (posX <= 0 || posX >= MAPWIDTH - 1) {
			printf("没有可出生的地方\n");
			return;
		}
	}
}

// 一键传送
void CTank::helpPlayer()
{
	m_vecTank[0]->curHP = 0;
	if (m_vecTank.size() > 2 && !m_vecTank[1]->isNPC) {
		m_vecTank[1]->curHP = 0;
	}
}


