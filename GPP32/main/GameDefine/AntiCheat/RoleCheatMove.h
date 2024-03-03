#pragma once
#include "../../main.h"

struct RoleCheatMove : II::Object {

	char checkRole[0x000004];
	float startFlyJumpTime;
	float checkUserJumpPointDelayTime;
	UnityResolve::UnityType::Vector3 startFlyJumpPoint;
	int reCheckFlyJumpNum;
	float delayCheckTime;
	float prevCheckY;
	UnityResolve::UnityType::Vector3 oldCheckPoint;
	int moveXZErrorCount;
	int moveYErrorCount;
	int cheatCarId;
	bool isStartRecord;
	char isStartRecord_[0x000003];
	UnityResolve::UnityType::Vector3 lastRecordPoint;
	char rolePath[0x000004];
	char addPathTime[0x000004];
	int movePathErrorCount;
	float checkMoveSpeed;
	bool IsHeavyAttack;
	char IsHeavyAttack_[0x000003];
	float WaitGunFightMoveMoveRoleTime;
	float userCannonFlyTime;
	UnityResolve::UnityType::Vector3 lastFlyMovePoint;
	int checkOldUseCar;
	int carErrorCount;
	UnityResolve::UnityType::Vector3 prevCheckXZ;
	UnityResolve::UnityType::Vector3 oldRoleCheckMovePoint;
	float cheatMaxSpeedTime;
	float cheatMaxSpeed;
	bool lastUseBuff;
	char lastUseBuff_[0x000003];
	float moveErrorCountTime;
	int moveSpeedErrorCount;
	float markRate;
	float checkCheatTime;
	float avgMoveDistance;
	char moveDistanceQueue[0x000004];
	float carErrorTime;
	bool isCheatCar;
	char isCheatCar_[0x000003];
	int moveDistanceError;
	UnityResolve::UnityType::Vector3 lastMovePoint;
	float delayCheckMoveTime;
	int moveTenError;
	float waitCheckTime;
	float YPointErrorCount;
	int pointYErrorCount;
	UnityResolve::UnityType::Vector3 lastLightningPoint;
	int lightningYErrorNum;
	float lastUseTime;
	char lightningErrorInfo[0x000004];

};