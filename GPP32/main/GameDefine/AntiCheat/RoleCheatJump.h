#pragma once
#include "../../main.h"

struct RoleCheatJump : II::Object {

	char checkRole[0x000004];
	int errorCount;
	int MAX_ERROR_COUNT;
	char isSendData[0x000004];

};