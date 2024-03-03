#pragma once
#include "../../main.h"
struct RoleCheatAction : II::Object {

	char checkRole[0x000004];
	int startJumpNum;
	UnityResolve::UnityType::Vector3 firstStartJumpPoint;
	float maxJumpHeight;
	int errorJumpNum;
	float lostClientToRoleMoveYTime;
	int clientToRoleMoveY;
	UnityResolve::UnityType::Vector3 lastRolePoint;
	int glideMoveErrorCount;
	int flyJumpStateCheatCount;
	float useParabolaMoveTime;
	char skillCheckCDDic[0x000004];

};