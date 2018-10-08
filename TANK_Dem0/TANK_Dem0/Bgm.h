#pragma once
#include <Windows.h>
#include <time.h>
#include <string>
#include<iostream>  
#include <sstream> 
#pragma comment(lib, "winmm.lib") 
using namespace std;
using std::string;

#define BGM_START_BATTLE   1
#define BGM_BULLET_HIT_NPC     2
#define BGM_KILLED_BY_PALYER 3
#define BGM_BULLET_HIT_ME  4
#define BGM_PLAYER_TANK_DIE  5
#define BGM_REQUEST_ASSIST    6
#define BGM_SUCCESS   7
#define BGM_FAIL      8
#define BGM_DROP_FLAG  9
#define BGM_BOMB   10
#define BGM_PAGE    11
#define BGM_CLICK   12
#define BGM_STAMP   13
#define BGM_STONE   14
#define BGM_BRICK   15
#define BGM_GRASS   16
#define BGM_RIVER   17
#define BGM_ICE     18
#define BGM_DONE    19
#define BGM_READY    20
#define BGM_JOIN     21
#define BGM_LEAVE    22
#define BGM_BYE      23
#define BGM_KAQSA    24
#define BGM_WELCOME  25

// ±≥æ∞“Ù¿÷
class CBgm
{
public:
	CBgm();
	~CBgm();
	static void play(int type);
};


