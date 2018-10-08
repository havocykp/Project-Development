#include "Bullet.h"

// 构造函数
CBullet::CBullet(CMaps* pMaps)
{
	m_pMaps = pMaps;
}

// 析构函数
CBullet::~CBullet()
{
}

// 子弹---障碍物
void CBullet::drawBullet(bool isShow)
{
	int posX;
	int posY;
	int color;

	for (size_t i = 0; i < m_vecBullet.size(); i++)
	{
		if (!m_vecBullet[i]->isValid) {
			continue;
		}
		posX = m_vecBullet[i]->posX;
		posY = m_vecBullet[i]->posY;
		color = m_vecBullet[i]->color;
		if (m_pMaps->m_nMap[posY][posX] == MAP_TREE) {
			// 树林要隐藏子弹： 不显示也不擦除
		}
		else if (isShow) {
			if ((m_pMaps->m_nMap[posY][posX] == MAP_GRASS) || (m_pMaps->m_nMap[posY][posX] == MAP_ICE) || (m_pMaps->m_nMap[posY][posX] == MAP_RIVER)) {
				// 显示子弹（经过后擦除子弹位置，植物被破坏， 但是要恢复被破坏的植物）
				m_pMaps->m_vecDamagedPlant.push_back({ posX, posY }); // 同步受损植物
			}
			m_pMaps->printChar(posX, posY, "●", color);
		}
		else {
			if ((m_pMaps->m_nMap[posY][posX] == MAP_GRASS) || (m_pMaps->m_nMap[posY][posX] == MAP_ICE) || (m_pMaps->m_nMap[posY][posX] == MAP_RIVER)) {//擦除位置有植物
				m_pMaps->m_vecDamagedPlant.push_back({ posX, posY });  // 同步受损植物
			}
			m_pMaps->printChar(posX, posY, "  ", color);
		}


	}
}

// 子弹----坦克
void CBullet::removeInvalidBullet()
{
	int posX;
	int posY;

	for (size_t i = 0; i < m_vecBullet.size(); i++)
	{
		// 如果子弹有效，则继续
		if (!m_vecBullet[i]->isValid) {
			continue;
		}
		posX = m_vecBullet[i]->posX;
		posY = m_vecBullet[i]->posY;

		// 坦克地图检测
		if (m_pMaps->m_pTankMap[posY][posX] != NULL) { // 击中了坦克
			BULLET* pBullet = m_vecBullet[i];
			TANK* pTank = m_pMaps->m_pTankMap[posY][posX];
			if (pBullet->isNPC != pTank->isNPC) { // 阵营不同： 减血
				pTank->curHP -= pBullet->power;
				if (pTank->isNPC && pTank->curHP > 10) {
					CBgm::play(BGM_BULLET_HIT_NPC);//bgm
				}
				else {
					CBgm::play(BGM_BULLET_HIT_ME);//bgm
					pTank->curSpeed -= 2;
				}

			}
			// 子弹失效
			m_vecBullet[i]->isValid = false;
			m_pMaps->m_pBulletMap[posY][posX] = NULL;
			continue;
		}
		// 障碍物检测
		else if (m_pMaps->m_nMap[posY][posX] == MAP_HEART) { // 打击了老巢： 子弹失效
			m_vecBullet[i]->isValid = false;
			//m_pMaps->m_pBulletMap[posY][posX] = NULL;
			m_pMaps->m_isHeartBroken = true;
			continue;
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_STONE) { // stone: 子弹失效
			m_vecBullet[i]->isValid = false;
			m_pMaps->m_pBulletMap[posY][posX] = NULL;
			continue;
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_SPACE) { // 空区域：不操作 
															
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_BRICK) {// 砖块： 都消失
			m_pMaps->m_nMap[posY][posX] = MAP_SPACE;
			m_pMaps->reDrawMapPoint(posY, posX);
			m_vecBullet[i]->isValid = false;
			m_pMaps->m_pBulletMap[posY][posX] = NULL;
			continue;
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_TREE) {// 树林
														  
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_ICE || m_pMaps->m_nMap[posY][posX] == MAP_GRASS || m_pMaps->m_nMap[posY][posX] == MAP_RIVER) { //草地和冰块
																																				   
		}
		else if (posX < 0 || posX >= MAPWIDTH || posY < 0 || posY >= MAPHEIGHT) {
			//printf("子弹坐标异常：（%d, %d）\n", posX, posY);

		}
		else {
			//printf(" 尚未定义的地图静态对象： removeInvalidBullet()\n");
			int map = m_pMaps->m_nMap[posY][posX];
			//printf(" x = %d y = %d  %d\n", posX, posY, map);
		}
	}
	// 删除无效子弹
	for (vector<BULLET*>::iterator itr = m_vecBullet.begin(); itr != m_vecBullet.end(); ++itr) {
		if (!(*itr)->isValid) {
			m_vecBullet.erase(itr);
			break;
		}
	}

	// 更新子弹地图
	for (int i = 0; i < MAPHEIGHT; i++) {
		for (int j = 0; j < MAPWIDTH; j++) {
			m_pMaps->m_pBulletMap[i][j] = NULL;
		}
	}
	for (size_t i = 0; i < m_vecBullet.size(); i++)
	{
		if (!m_vecBullet[i]->isValid) {
			continue;
		}
		posX = m_vecBullet[i]->posX;
		posY = m_vecBullet[i]->posY;
		m_pMaps->m_pBulletMap[posY][posX] = m_vecBullet[i];
	}
}

// 检测子弹是否可以飞
bool CBullet::runBullet()
{
	// 擦除所有子弹
	drawBullet(false);
	for (size_t i = 0; i < m_vecBullet.size(); i++)
	{
		if (!m_vecBullet[i]->isValid) {
			continue;
		}
		// 飞行速度设置
		clock_t start_time = m_vecBullet[i]->last_fly_time;
		clock_t end_time = clock();
		if (end_time - start_time < 50) {
			//continue;
		}
		m_vecBullet[i]->last_fly_time = end_time;

		// 子弹坐标
		int posX = m_vecBullet[i]->posX;
		int posY = m_vecBullet[i]->posY;
		// 修改坐标
		switch (m_vecBullet[i]->dir) {
		case DIR_UP:
			posY--;
			break;
		case DIR_DOWN:
			posY++;
			break;
		case DIR_LEFT:
			posX--;
			break;
		case DIR_RIGHT:
			posX++;
			break;
		default:
			//printf("错误的子弹方向属性：drawbullet\n");
			return false;
			break;
		}
		m_vecBullet[i]->posX = posX;
		m_vecBullet[i]->posY = posY;
		// 判断前进后是否将碰到子弹, 不能在removeInvalidBullet中检测；
		if (m_pMaps->m_pBulletMap[posY][posX]) { // 子弹 -- 子弹检测 : 两枚子弹都失效
			m_vecBullet[i]->isValid = false;
			m_pMaps->m_pBulletMap[posY][posX]->isValid = false;
		}
	}
	// 移除无效子弹
	removeInvalidBullet();
	// 显示
	drawBullet(true);
	return true;
}

// 检测子弹是否可以发射
void CBullet::canShoot(vector<TANK*>& m_vecTank)
{
	clock_t start_time = m_vecTank[0]->last_shoot_time;
	clock_t end_time = clock();
	if (m_vecTank[0]->isAlive && end_time - start_time > 2000 && m_vecTank[0]->ready == false) {
		//CBgm::play(BGM_READY);//bgm
		m_vecTank[0]->ready = true;
	}
}

// 发射子弹
void CBullet::shootBullet(vector<TANK *>& m_vecTank, int nTankIndex)
{
	// 射速设置
	clock_t start_time = m_vecTank[nTankIndex]->last_shoot_time;
	clock_t end_time = clock();
	if (m_vecTank[nTankIndex]->isNPC && end_time - start_time < 4000) {
		return;
	}
	else if (!m_vecTank[nTankIndex]->isNPC && end_time - start_time < 1000) {
		return;
	}
	else if (!m_vecTank[nTankIndex]->isAlive) {
		return;
	}
	// 玩家坦克发射子弹
	if (!m_vecTank[nTankIndex]->isNPC) {
		CBgm::play(BGM_BOMB);//bgm
	}
	m_vecTank[nTankIndex]->last_shoot_time = end_time;
	m_vecTank[nTankIndex]->ready = false;

	int posX = m_vecTank[nTankIndex]->posX;
	int posY = m_vecTank[nTankIndex]->posY;
	int dir = m_vecTank[nTankIndex]->dir;
	switch (dir) {
	case DIR_UP:
		posY -= 2;
		break;
	case DIR_DOWN:
		posY += 2;
		break;
	case DIR_LEFT:
		posX -= 2;
		break;
	case DIR_RIGHT:
		posX += 2;
		break;
	default:
		//printf("错误的子弹初始方向：shootBullet\n");
		return;
		break;
	}

	// 子弹数据初始化
	BULLET* pBullet = (BULLET*)malloc(sizeof(BULLET));
	pBullet->posX = posX;
	pBullet->posY = posY;
	pBullet->dir = dir;
	pBullet->isNPC = m_vecTank[nTankIndex]->isNPC;
	pBullet->tankId = nTankIndex;
	pBullet->last_fly_time = 0;
	pBullet->power = 50;
	pBullet->color = m_vecTank[nTankIndex]->color;

	m_vecBullet.push_back(pBullet);
	// 立即移除无效子弹
	removeInvalidBullet();
	// 显示
	drawBullet(true);
}