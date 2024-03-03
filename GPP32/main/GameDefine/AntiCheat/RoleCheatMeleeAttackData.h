#pragma once
#include "../../main.h"

struct RoleCheatMeleeAttackData : II::Object {

	char checkRole[0x000004];
	int lightAttackCountt;
	int heavyAttackCount;
	UnityResolve::UnityType::String* panchStr;
	UnityResolve::UnityType::String* heavyStr;
	UnityResolve::UnityType::String* lightStr;
	float checkHeavyAttackSuccessTime;
	int heavyAttackSuccessCheatNum;
	float checkHitPartCDTime;
	char hitPartCDErrorNum[0x000004];
	char lastAttackType[0x000004];
	char isLastCancelHeavyAttack[0x000004];

};