#pragma once
#include "../../main.h"

struct RoleCheatMTP : II::Object {

	char checkRole[0x000004];
	float heartBeatTime;
	float prevMTPTime;
	bool isStartHeartBeat;
	char isStartHeartBeat_[0x000003];
	int mtpCheatCount;
	char closeClientCheatCount[0x000004];

};