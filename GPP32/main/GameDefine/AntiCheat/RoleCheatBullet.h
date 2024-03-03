#pragma once
#include "../../main.h"

struct RoleCheatBullet : II::Object {

	char checkRole[0x000004];
	char raycastCheckList[0x000004];
	char crossFireData[0x000004];
	int checkDownHpRolePoint;
	int checkBulletHitPointNum;
	int sendCount;
	float totalSendTime;
	UnityResolve::UnityType::Vector3 lastHitPint;
	float bulletHitDistanceErrorCount;
	float totalBulletNum;
	float totalBulletDistance;
	float checkTotalDistanceError;
	float lastQuaternionX;
	float lastQuaternionY;
	float checkQuaternionX;
	float checkQuaternionY;
	float totalCameraMoveX;
	float totalCameraMoveY;
	int fireNum;
	int errorCount;
	int bulletMoveTimeErrorNum;
	UnityResolve::UnityType::Transform* checkBulletTran;
	int firePointToRolePoint;
	float hitRolePointDelayTime;
	int hitRolePointErrorNum;
	char hitPartDic[0x000004];
	int SendBulletCount;
	char bulletFireList[0x000004];
	UnityResolve::UnityType::String* currentHwSign;
	char hiddenBulletCheatList[0x000004];
	float lastErrorTime;
	int bulletErrorCount;
	int HolySwordBuffError;
	int holySwardBuffNum;
	int attackNum;
	int holySwardErrorNum;
	float waitPintOrKillDelayTime;
	int lastKillNum;
	int lastToWeakNum;
	char pingKillErrorNum[0x000004];

};