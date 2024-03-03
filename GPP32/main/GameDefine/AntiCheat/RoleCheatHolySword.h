#pragma once
#include "../../main.h"

struct RoleCheatHolySword : II::Object {

	char checkRole[0x000004];
	int holySwordFireTick;
	int holySwordFireErrorCount;
	int holySwordFireErrorNum;
	float time;
	char numTime[0x000004];

};