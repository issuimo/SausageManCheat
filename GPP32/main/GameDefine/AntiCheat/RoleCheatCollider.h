#pragma once
#include "../../main.h"

struct RoleCheatCollider : II::Object {

	char roleNet[0x000004];
	char gameWorld[0x000004];
	float checkDelayTime;
	char cheatCollider[0x000004];
	char cheatColliderData[0x000004];

};