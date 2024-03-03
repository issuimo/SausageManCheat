#pragma once
#include "../../main.h"

struct RoleCheatKill : II::Object {

	char checkRole[0x000004];
	char killTimes[0x000004];
	int killErrorCount;
	char damageTimes[0x000004];
	int curDamageIndex;
	char curDamageServerTime[0x000004];

};