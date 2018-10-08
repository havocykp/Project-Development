#pragma once
#include <sstream>
#include "Data.h"
#include "Maps.h"
#include "Tank.h"
#include "Bullet.h"
#include "Bgm.h"

class CCtrl
{
public:
	CCtrl();
	~CCtrl();
public:
	// 开始玩坦克游戏
	void playTank(int gameType);
	// 方向键转方向宏
	int dirKey2DirNum(char keyWord);
	// 设置控制台大小
	void initConsoleWindow();
	// 初始化输入法
	void initInputShift();
	// 显示主页菜单
	void showMenu();
	// 获取用户的游戏主菜单选项
	int getMenuChoice();
	// 显示选择玩家数量菜单
	void showPlayerCountMenu();
	// 获取用户的玩家数量
	int getPlayerCount();
	// 显示并获取现有的关卡菜单 maps参数，只读
	int chooseLevelPassMenu(CMaps& maps);
	// 获取下一个关卡
	int getNextLevel(int nCurrent);
	// 存档
	void saveArchive(CMaps& maps, CTank& tanks, CBullet& bullets);
	// 读档
	void readArchive(CMaps& maps, CTank& tanks, CBullet& bullets);
	// 清空全局变量
	void clearGlobalData(CMaps& maps, CTank& tanks, CBullet& bullets);
	// 右侧显示游戏信息
	void showGameInfo(CTank& tanks, CBullet& bullets);


};


