#pragma once
#include "../main.h"

struct BulletSpeedAndGravity : II::Object {
	float gravity;
	float bulletSpeed;
	char flyTime[0x000004];
};