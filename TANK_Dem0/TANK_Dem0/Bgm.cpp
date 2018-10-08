#include "Bgm.h"


// 构造函数
CBgm::CBgm()
{
}

// 析构函数
CBgm::~CBgm()
{
}

// 播放背景乐
void CBgm::play(int type)
{
	stringstream stream;
	clock_t time = clock();
	string fullPath("bgm\\");
	switch (type) {
	case  BGM_START_BATTLE: //开始游戏
		fullPath.append("start_battle_0");
		stream << clock() % 4 + 1;
		fullPath.append(stream.str());
		break;
	case BGM_KILLED_BY_PALYER: //击杀敌方坦克
		fullPath.append("enemy_killed_by_player_0");
		stream << clock() % 5 + 1;
		fullPath.append(stream.str());
		break;
	case BGM_BULLET_HIT_NPC: //打中了敌人
		fullPath.append("bullet_hit_npc_0");
		stream << clock() % 3 + 1;
		fullPath.append(stream.str());
		break;
	case BGM_REQUEST_ASSIST://被请求协助
		fullPath.append("request_assist");
		break;
	case BGM_BULLET_HIT_ME: //玩家被击中 : 成员受伤或死亡
		fullPath.append("bullet_hit_me_0");
		stream << clock() % 8 + 1;
		fullPath.append(stream.str());
		break;
	case BGM_PLAYER_TANK_DIE: //坦克凉了
		fullPath.append("tank_die");
		break;
	case BGM_SUCCESS:
		fullPath.append("success_0");
		stream << clock() % 4 + 1;
		fullPath.append(stream.str());
		break;
	case BGM_FAIL:
		fullPath.append("fail_0");
		stream << clock() % 5 + 1;
		fullPath.append(stream.str());
		break;
	case BGM_DROP_FLAG:
		fullPath.append("drop_flag");
		break;
	case BGM_BOMB:
		fullPath.append("bomb");
		break;
	case BGM_PAGE:
		fullPath.append("page");
		break;
	case BGM_CLICK:
		fullPath.append("click");
		break;
	case BGM_STAMP:
		fullPath.append("stamp");
		break;
	case BGM_STONE:
		fullPath.append("stone");
		break;
	case BGM_BRICK:
		fullPath.append("brick");
		break;
	case BGM_GRASS:
		fullPath.append("grass");
		break;
	case BGM_RIVER:
		fullPath.append("river");
		break;
	case BGM_ICE:
		fullPath.append("ice");
		break;
	case BGM_DONE:
		fullPath.append("done");
		break;
	case BGM_READY:
		fullPath.append("ready");
		break;
	case BGM_JOIN:
		fullPath.append("join");
		break;
	case BGM_LEAVE:
		fullPath.append("leave");
		break;
	case BGM_BYE:
		fullPath.append("byebye");
		break;
	case BGM_KAQSA:
		fullPath.append("katusha");
		break;
	case BGM_WELCOME:
		fullPath.append("welcome");
	}
	fullPath.append(".wav");
	PlaySoundA(fullPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}
