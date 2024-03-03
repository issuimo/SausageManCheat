#pragma once
#include "../../main.h"

struct RoleCheatUseItem : II::Object {

	char checkRole[0x000004];
	int bombRangeCheatNum;
	int bombMoveError;
	float _StartUseItemTime_k__BackingField;
	int cheatUseItemTime;
	char flyHookDisErrorTime[0x000004];

};