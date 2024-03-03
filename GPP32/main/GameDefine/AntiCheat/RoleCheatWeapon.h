#pragma once
#include "../../main.h"

struct RoleCheatWeapon : II::Object {

	char checkRole[0x000004];
	char checkFireErrorData[0x000004];
	bool checkBulletFireState;
	char checkBulletFireState_[0x000003];
	float fireStateChangeTime;
	float prevFireTime;
	float prevFireWeaponId;
	UnityResolve::UnityType::Quaternion FireRy;
	UnityResolve::UnityType::Quaternion FireRx;
	float FireRyStartTime;
	float FireRyCheckValue;
	int FireRyCheckNum;
	float FireRyLimitValue;
	float FireRyCheckValueErrorNum;
	char soCheatZiZiBeng[0x000004];
	char demonMacheteCreateTimeDic[0x000004];

};