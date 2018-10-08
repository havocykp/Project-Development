#include "Maps.h"
#include "Bgm.h"

// 构造函数
CMaps::CMaps()
{
	m_nMap[MAPHEIGHT][MAPWIDTH] = { 0 };
	m_pTankMap[MAPHEIGHT][MAPWIDTH] = { NULL };
	m_pBulletMap[MAPHEIGHT][MAPWIDTH] = { NULL };
	m_isHeartBroken = false;
}

// 析构函数
CMaps::~CMaps()
{

}

// 初始化外墙
void CMaps::initOuterWall()
{
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			if (row == 0 || row == MAPHEIGHT - 1 || col == 0 || col == MAPWIDTH - 1) {
				m_nMap[row][col] = MAP_STONE;
			}
			else {
				m_nMap[row][col] = MAP_SPACE;
			}
		}
	}

}

// 初始化老巢
void CMaps::initHeart()
{
	int Y = MAPHEIGHT - 3;
	int X = MAPWIDTH / 2 - 1;
	m_nMap[Y][X] = MAP_HEART;
	//保护墙
	m_nMap[Y + 1][X + 1] = MAP_BRICK;
	m_nMap[Y + 1][X - 1] = MAP_BRICK;
	m_nMap[Y][X + 1] = MAP_BRICK;
	m_nMap[Y][X - 1] = MAP_BRICK;
	m_nMap[Y - 1][X] = MAP_BRICK;
	m_nMap[Y - 1][X + 1] = MAP_BRICK;
	m_nMap[Y - 1][X - 1] = MAP_BRICK;

}

// 初始化地图中的障碍物
void CMaps::initStaticMapData()
{
	initOuterWall();
	initHeart();
	for (int col = 1; col < MAPWIDTH - 1; col++) {
		// 草地
		m_nMap[4][col] = MAP_GRASS;
		m_nMap[5][col] = MAP_GRASS;
	}

	for (int col = 1; col < MAPWIDTH - 1; col++) {
		// 树林
		m_nMap[8][col] = MAP_TREE;
		m_nMap[9][col] = MAP_TREE;
	}

	for (int col = 1; col < MAPWIDTH - 1; col++) {
		// 雪地
		m_nMap[12][col] = MAP_ICE;
		m_nMap[13][col] = MAP_ICE;
	}


	for (int col = 0; col < 10; col++) {
		// 土墙
		m_nMap[17][col] = MAP_STONE;
		m_nMap[18][col] = MAP_STONE;
	}

	for (int col = MAPWIDTH - 15; col < MAPWIDTH - 1; col++) {
		// 河流
		m_nMap[17][col] = MAP_RIVER;
		m_nMap[18][col] = MAP_RIVER;
	}
	for (int row = 20; row < 30; row++) {
		for (int col = 1; col < MAPWIDTH - 1; col++) {
			// 砖墙
			m_nMap[row][col] = MAP_BRICK;
		}
	}
}

void CMaps::showNeedStaticObj()
{
	int nDestPosX = MAPWIDTH + 2;
	printChar(nDestPosX, 0, "＜ 返回主界面 ＞  ", COLOR_RED); // 空地

	printChar(nDestPosX, 2, "□ 空地", COLOR_WHITE); // 空地
	printChar(nDestPosX, 4, "■ 石块", COLOR_GRAY);  // 石块
	printChar(nDestPosX, 6, "■ 砖块", COLOR_RED_LIGHT); // 砖块
	printChar(nDestPosX, 8, "∷ 草地", COLOR_GREEN);   // 草
	printChar(nDestPosX, 10, "※ 雪地", COLOR_WHITE);   // 雪
	printChar(nDestPosX, 12, "↑ 树木", COLOR_GREEN_LIGHT); // 树
	printChar(nDestPosX, 14, "≈ 河流", COLOR_GRAY);   // 河流

	printChar(nDestPosX, 27, "  ① 重新绘制 ", COLOR_RED);
	printChar(nDestPosX, 28, "  ② 打开内置地图 ", COLOR_RED);
	printChar(nDestPosX + 3, 29, "←", COLOR_GREEN); printChar(nDestPosX + 4, 29, "  ", COLOR_GREEN); printChar(nDestPosX + 5, 29, "→", COLOR_GREEN);

	printChar(nDestPosX, 30, "  ③ 保存未编号 ", COLOR_RED);
	printChar(nDestPosX, 31, "  ④ 按编号保存 ", COLOR_RED);
	printChar(nDestPosX + 3, 32, "←", COLOR_GREEN); printChar(nDestPosX + 4, 32, "  ", COLOR_GREEN); printChar(nDestPosX + 5, 32, "→", COLOR_GREEN);

}

// 自定义地图
void CMaps::customMapData()
{
	int nColorBoxPosX = MAPWIDTH + 1; //同showNeedStaticObj()保持一致

	int nBrushColorNum = 0; //鼠标的刷子颜色
	int mousePosX = 0;
	int mousePosY = 0;

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);


	bool activeScrollPainting = false;
	int nLevelPass = 1; // 强制要保存的关卡号
	int nImportLevelPass = -1; // 想要导入的关卡号
	while (1)
	{
		ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);
		if (stcRecord.EventType == MOUSE_EVENT) {
			MOUSE_EVENT_RECORD mer = stcRecord.Event.MouseEvent;
			switch (mer.dwEventFlags)
			{
			case 0: // 单击
				if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) { // 左键单击

					if (mousePosX >= nColorBoxPosX) { // 点击障碍
						switch (mousePosY) {
						case 0: // 返回
							//CBgm::play(BGM_CLICK);//bgm
							return;
						case 2:
							nBrushColorNum = MAP_SPACE;
							//CBgm::play(BGM_STAMP);//bgm
							break;
						case 4:
							nBrushColorNum = MAP_STONE;
							//CBgm::play(BGM_STONE);//bgm
							break;
						case 6:
							nBrushColorNum = MAP_BRICK;
							//CBgm::play(BGM_BRICK);//bgm
							break;
						case 8:
							nBrushColorNum = MAP_GRASS;
							//CBgm::play(BGM_GRASS);//bgm
							break;
						case 10:
							nBrushColorNum = MAP_ICE;
							//CBgm::play(BGM_ICE);//bgm
							break;
						case 12:
							nBrushColorNum = MAP_TREE;
							//CBgm::play(BGM_ICE);//bgm
							break;
						case 14:
							nBrushColorNum = MAP_RIVER;
							//CBgm::play(BGM_RIVER);//bgm
							break;
						case 27: // 新建关卡图
							//CBgm::play(BGM_CLICK);//bgm
							for (int row = 4; row < MAPHEIGHT - 4; row++) {
								for (int col = 1; col < MAPWIDTH - 1; col++) {
									m_nMap[row][col] = 0;
									reDrawMapPoint(row, col);
								}
							}
							break;
						case 28: // 导入关卡
							printChar(MAPWIDTH + 2, 28, "请选择关卡", COLOR_RED);
							Sleep(1000);
							printChar(MAPWIDTH + 2, 28, "打开内置地图", COLOR_GRAY);
							break;
						case 29: // 选择导入的关卡号
						{
							//CBgm::play(BGM_PAGE);//bgm

							bool hasFind = false;
							int  loopTimes = 0;
							while (!hasFind) {
								loopTimes++;
								if (loopTimes > 40) { // 找不到文件了
									break;
								}
								if (mousePosX == MAPWIDTH + 5) { // 减小关卡数字；
									nImportLevelPass--;
									nImportLevelPass = nImportLevelPass >= 1 ? nImportLevelPass : 38;

								}
								else if (mousePosX == MAPWIDTH + 7) { // 增大关卡数字
									nImportLevelPass++;
									nImportLevelPass = nImportLevelPass <= 38 ? nImportLevelPass : 1;
								}
								int nFileNum = nImportLevelPass;
								stringstream stream;
								stream << nFileNum;
								string  strFile = stream.str();
								string  strFileCopy = strFile;
								char * strFileNum = (char *)strFileCopy.c_str();  // UI显示使用
								strFile.insert(0, "map\\"); // 拼凑文件路径
								const char * pFilename = strFile.c_str();
								FILE * pFile;
								errno_t errNum = fopen_s(&pFile, pFilename, "rb");

								if (errNum == 0) { // 文件存在: 读取-》导入
									hasFind = true;
									fread_s(m_nMap, MAPHEIGHT*MAPWIDTH * sizeof(int), sizeof(int), MAPWIDTH*MAPHEIGHT, pFile);
									fclose(pFile);
									// 显示当前的关卡号码
									printChar(MAPWIDTH + 6, 29, "  ", COLOR_GRAY); // 清空
									printChar(MAPWIDTH + 6, 29, strFileNum, COLOR_GRAY);
									// 重画
									for (int i = 4; i < MAPHEIGHT - 4; i++) {
										for (int j = 0; j < MAPWIDTH; j++) {
											reDrawMapPoint(i, j);
										}
									}

								}
							}
							if (!hasFind) {
								printChar(MAPWIDTH + 6, 29, "NO", COLOR_GRAY);
							}

						}
						break;
						case 30: // 保存为关卡地图: 自动编号
							//CBgm::play(BGM_DONE);//bgm
							printChar(MAPWIDTH + 2, 30, "保存中...    ", COLOR_RED); // 坐标对应菜单

							for (int i = 1; i < 10; i++) {
								int nFileNum = i;
								stringstream stream;
								stream << nFileNum;
								string  strFile = stream.str();
								strFile.insert(0, "map\\"); // 拼凑文件路径
								const char * pFilename = strFile.c_str();
								FILE * pFile;
								errno_t errNum = fopen_s(&pFile, pFilename, "rb");

								if (errNum != 0) { // 文件不存在
									FILE* fpFile;
									fopen_s(&fpFile, pFilename, "wb");
									// 写入静态地图数据
									initHeart(); //home
									fwrite(m_nMap, sizeof(int), MAPWIDTH* MAPHEIGHT, fpFile);
									fclose(fpFile);
									printChar(MAPWIDTH + 2, 30, "保存成功...        ", COLOR_RED);
									Sleep(1000);
									printChar(MAPWIDTH + 2, 30, "保存未编号", COLOR_GRAY); // 坐标对应菜单
									break;
								}
								else {
									fclose(pFile);
								}

							}
							break;
						case 31: // 强制保存为关卡 nLevelPass
							//CBgm::play(BGM_DONE);//bgm
							printChar(MAPWIDTH + 2, 31, "保存中...        ", COLOR_RED);
							// 拼凑文件路径
							{
								stringstream stream;
								stream << nLevelPass;
								string  strLevel = stream.str();
								strLevel.insert(0, "map\\");

								FILE* fpFile;
								fopen_s(&fpFile, (char *)strLevel.c_str(), "wb");
								// 写入静态地图数据
								initHeart(); //home
								fwrite(m_nMap, sizeof(int), MAPWIDTH* MAPHEIGHT, fpFile);
								fclose(fpFile);
							}
							printChar(MAPWIDTH + 2, 31, "保存成功...        ", COLOR_RED);
							Sleep(1000);
							printChar(MAPWIDTH + 2, 31, "选择编号保存", COLOR_GRAY); // 坐标对应菜单
							break;
						case 32: // 调节关卡数字

							//CBgm::play(BGM_PAGE);//bgm

							if (mousePosX == MAPWIDTH + 5) { // 减小关卡数字；
								nLevelPass--;
								nLevelPass = nLevelPass >= 1 ? nLevelPass : 1;

							}
							else if (mousePosX == MAPWIDTH + 7) { // 增大关卡数字
								nLevelPass++;
								nLevelPass = nLevelPass <= 36 ? nLevelPass : 36;
							}
							{
								printChar(MAPWIDTH + 6, 32, "  ", COLOR_GRAY); // 清空数字， 防止10以上数字残留
								stringstream stream;
								stream << nLevelPass;
								string  strLevel = stream.str();
								printChar(MAPWIDTH + 6, 32, (char *)strLevel.c_str(), COLOR_GRAY);
							}
							break;
						default:
							break;
						}
					}
					else if (mousePosX > 0 && mousePosX < MAPWIDTH - 1 && mousePosY > 3 && mousePosY < MAPHEIGHT - 4) {
						// 内容区单击
						if (nBrushColorNum == m_nMap[mousePosY][mousePosX]) { // 取消
							m_nMap[mousePosY][mousePosX] = 0;
						}
						else {
							m_nMap[mousePosY][mousePosX] = nBrushColorNum;
							//bgm_start
							switch (nBrushColorNum) {
							case MAP_SPACE:
								//CBgm::play(BGM_STAMP);//bgm
								break;
							case MAP_STONE:
								//CBgm::play(BGM_STONE);//bgm
								break;
							case  MAP_BRICK:
								//CBgm::play(BGM_BRICK);//bgm
								break;
							case MAP_GRASS:
								//CBgm::play(BGM_GRASS);//bgm
								break;
							case MAP_ICE:
								//CBgm::play(BGM_ICE);//bgm
								break;
							case MAP_TREE:
								//CBgm::play(BGM_ICE);//bgm
								break;
							case MAP_RIVER:
								//CBgm::play(BGM_RIVER);//bgm
								break;
							}

							//bgm_end
						}
						reDrawMapPoint(mousePosY, mousePosX);
					}
				}
				else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
				{
					// 右键单击: 结束编辑编辑
					return;
				}
				break;
			case DOUBLE_CLICK:
				// 内容区双击 
				if (mousePosX > 0 && mousePosX < MAPWIDTH - 1 && mousePosY > 3 && mousePosY < MAPHEIGHT - 4) {
					m_nMap[mousePosY][mousePosX] = nBrushColorNum;
					reDrawMapPoint(mousePosY, mousePosX);
					// 激活或者取消滑动绘图
					activeScrollPainting = !activeScrollPainting;
				}
				break;
			case MOUSE_MOVED:
				mousePosX = mer.dwMousePosition.X / 2;
				mousePosY = mer.dwMousePosition.Y;
				if (mousePosX > 0 && mousePosX < MAPWIDTH - 1 && mousePosY > 3 && mousePosY < MAPHEIGHT - 4) {
					// 内容区
					if (activeScrollPainting) { // 滑动绘图
						m_nMap[mousePosY][mousePosX] = nBrushColorNum;
						reDrawMapPoint(mousePosY, mousePosX);
					}
				}

				break;
			}
		}

		// 外墙变色，表明激活了滑动绘图
		for (int row = 0; row < MAPHEIGHT; row++) {
			for (int col = 0; col < MAPWIDTH; col++) {
				if (row == 0 || row == MAPHEIGHT - 1 || col == 0 || col == MAPWIDTH - 1) {
					if (activeScrollPainting) {
						CMaps::printChar(col, row, "■", rand() % 0xF + 0x0);
						printChar(1, MAPHEIGHT, "滑动绘图已开启: 双击关闭", COLOR_YELLOW);
					}
					else {
						CMaps::printChar(col, row, "■", COLOR_GRAY);
						printChar(1, MAPHEIGHT, "                                            ", COLOR_GRAY);
					}

				}
			}
		}

	}

}

// 参数（ posY,posX）
void CMaps::reDrawMapPoint(int row, int col)
{

	if (m_nMap[row][col] == MAP_STONE) {
		printChar(col, row, "■", COLOR_GRAY);
	}
	else if (m_nMap[row][col] == MAP_BRICK) {
		printChar(col, row, "■", COLOR_RED_LIGHT);
	}
	else if (m_nMap[row][col] == MAP_SPACE) {
		printChar(col, row, "  ", COLOR_BLACK);
	}
	else if (m_nMap[row][col] == MAP_GRASS) {
		printChar(col, row, "∷", COLOR_GREEN);
	}
	else if (m_nMap[row][col] == MAP_ICE) {
		printChar(col, row, "※", COLOR_WHITE);
	}
	else if (m_nMap[row][col] == MAP_TREE) {
		printChar(col, row, "↑", COLOR_GREEN_LIGHT);
	}
	else if (m_nMap[row][col] == MAP_RIVER) {
		printChar(col, row, "≈", COLOR_GRAY);
	}
	else if (m_nMap[row][col] == MAP_HEART) {
		printChar(col, row, "★", COLOR_RED);
	}
	else {
		printf("未定义的地图物体\n");
	}
}

// 打印地图中静态物体
void CMaps::drawMap()
{
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			reDrawMapPoint(row, col);
		}
	}

}

// 读取地图文件
void CMaps::readStaticMapFile(int nFileNum)
{
	// 先清空m_nMaps;
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			m_nMap[row][col] = 0;
		}
	}
	// 配凑地图文件路径
	stringstream stream;
	stream << nFileNum;
	string  strFile = stream.str();
	strFile.insert(0, "map\\"); // 拼凑文件路径
	const char * pFilename = strFile.c_str();
	FILE * pFile;
	fopen_s(&pFile, pFilename, "rb");
	// 地图g_nMap数据
	fread_s(m_nMap, sizeof(int)*MAPWIDTH*MAPHEIGHT, sizeof(int), MAPWIDTH*MAPHEIGHT, pFile);
	fclose(pFile);
}

// 恢复受损的植物
void CMaps::recoverDamagedPlant()
{
	for (vector<POS>::iterator it = m_vecDamagedPlant.begin(); it != m_vecDamagedPlant.end(); ) {
		int posX = it->posX;
		int posY = it->posY;
		if (m_nMap[posY][posX] != 0 && m_pBulletMap[posY][posX] == NULL && m_pTankMap[posY][posX] == NULL) {
			// 没有坦克或子弹时才恢复植物
			reDrawMapPoint(posY, posX);
			it = m_vecDamagedPlant.erase(it);
		}
		else {
			++it;
		}
	}

}

// 指定位置打印
void  CMaps::printChar(int posX, int posY, const char* pszChar, WORD wArr)
{
	// 1.设置光标属性
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	// 2.修改光标在控制台中的坐标位置
	COORD loc;
	loc.X = posX * 2;
	loc.Y = posY;
	// 设置颜色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wArr);
	// 3.设置光标在控制台中的坐标位置
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
	printf(pszChar);
}


