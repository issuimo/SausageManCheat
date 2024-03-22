#pragma once

#include "../../main.h"
#include "CarNet.h"
#include "SOCarDataConfig.h"

struct Car : II::MonoBehaviour {

	UnityResolve::UnityType::Transform* turnOverPoint;
	UnityResolve::UnityType::Transform* myTransform;
	UnityResolve::UnityType::Transform* myEffectParent;
	UnityResolve::UnityType::GameObject* myGameObj;
	UnityResolve::UnityType::Rigidbody* MyRigidbody;
	UnityResolve::UnityType::String* CarDataId;
	SOCarDataConfig* soCarData;
	UnityResolve::UnityType::String* CarSkinDataId;
	char soCarSkinData[0x000004];
	float _LocalTransformCD_k__BackingField;
	float _LocalTransTime_k__BackingField;
	float _LocalShiftDownTime_k__BackingField;
	float _LocalShiftBtnDownTime_k__BackingField;
	char MyCarCheck[0x000004];
	UnityResolve::UnityType::Transform* CameraTarget;
	UnityResolve::UnityType::Transform* LookTarget;
	UnityResolve::UnityType::String* deadEffectSign;
	bool closeDeadSoundTip;
	char closeDeadSoundTip_[0x000003];
	UnityResolve::UnityType::String* robotDeadEffectSign;
	char Roles[0x000004];
	char errorOffCarPos[0x000004];
	char carGetOffPos[0x000004];
	UnityResolve::UnityType::Transform* robotSit;
	char rolesObj[0x000004];
	char mCarTransform[0x000004];
	CarNet* MyCarNet;
	char MyBuggyCarControl[0x000004];
	char MyJetCarControl[0x000004];
	char MyMotorCarControl[0x000004];
	char animalController[0x000004];
	char flyVehicle[0x000004];
	char mDragon[0x000004];
	char mRobot[0x000004];
	char mShark[0x000004];
	UnityResolve::UnityType::String* DiverCarEffectName;
	char DiverCarEffectParentPoint[0x000001];
	bool isDownWater;
	char isDownWater_[0x000002];
	int carId;
	bool IsInWater;
	bool IsBuggy;
	bool IsJetCar;
	char IsJetCar_[0x000001];
	UnityResolve::UnityType::String* CarMoveSign;
	UnityResolve::UnityType::String* CarStopSign;
	bool isStart;
	bool isMoving;
	bool oldIsMoving;
	char oldIsMoving_[0x000001];
	UnityResolve::UnityType::GameObject* carMoveEff;
	UnityResolve::UnityType::GameObject* carStopEff;
	float carTime;
	float controlDeploy;
	float moveDeploy;
	UnityResolve::UnityType::String* RTPC_RPM;
	UnityResolve::UnityType::String* carBulletHitSound;
	UnityResolve::UnityType::String* hitSoundGroup;
	bool deadNotHideCollider;
	char deadNotHideCollider_[0x000003];
	float delaySpoliedTime;
	char userRoleEff[0x000004];
	char UserCarHideBox[0x000004];
	UnityResolve::UnityType::GameObject* forwardMoveEff;
	UnityResolve::UnityType::GameObject* checkInCar;
	char carLostLifeEffect[0x000004];
	UnityResolve::UnityType::Transform* offCarPos;
	char MySceneMarkObjs[0x000004];
	char mCarOcclusion[0x000004];
	int currentLostHpEffectIndex;
	char hitList[0x000004];
	float OffCarActiveDelay;
	int checkShowUserCarHideBox;
	int checkShowCarHit;
	float effDelayTime;
	bool isShowUserCarHideBox;
	bool isShowCarHit;
	char isShowCarHit_[0x000002];
	UnityResolve::UnityType::GameObject* carHitObj;
	UnityResolve::UnityType::Transform* carHitTran;
	char oldUserRole[0x000004];
	char mAuRole[0x000004];
	int currentUserRoleId;
	bool IsOcclusion;
	char IsOcclusion_[0x000003];
	float PhysicsDropTime;
	float TransformRobotDelayStopTime;
	bool IsClear;
	char IsClear_[0x000003];
	int currentCarState;
	UnityResolve::UnityType::Vector3 oldNowPoint;
	char CarShift[0x000004];
	char hitPropertyBlockList[0x000004];
	char JetCarJump[0x000004];
	char dinosaurSkin[0x000004];
	UnityResolve::UnityType::String* _AttackSign_k__BackingField;
	UnityResolve::UnityType::String* _MoveSign_k__BackingField;
	char OnSetDiverIndexCallBack[0x000004];
	int localCurrentCarState;
	float lastSyncShiftDownTime;
	float localCheckShiftDelayTime;
	char OnChangeUseRoleIdCallBack[0x000004];
	char OnUpdateCallBack[0x000004];
	UnityResolve::UnityType::Vector3 lastCarPos;
	float buggyStopSoundThreshold;
	float jetcarStopSoundThreshold;
	char useRole[0x000004];
	char userCarSpeedAnim[0x000004];
	float animDeployTime;
	float carForward;
	float carAccelBack;
	float carSteer;
	UnityResolve::UnityType::GameObject* triggerGameObj;
	char carHitRendererInfo[0x000004];
	float ShootTime;
	bool IsOpenGlitter;
	bool isGlitter;
	char isGlitter_[0x000002];
	float waitCloseShootTime;
	char oldHp[0x000004];

	static auto Update() -> void {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("Car");
		if (pClass) {
			std::vector<Car*> temp{};
			[&]() {
                __try {
                    [&]() {
                        for (const auto pRole : pClass->FindObjectsByType<Car *>()) {
                            if (!IsBadReadPtr(pRole, sizeof(Car))) {
                                temp.push_back(pRole);
                            }
                        }
                    }();
                } __except (EXCEPTION_EXECUTE_HANDLER) {
                    []() {
                        ERROR("Car-FindObjectsByType (except)");
                    }();
                }
            }();
			std::lock_guard lock(mutex);
			vector = temp;
		}
	}

	inline static std::mutex mutex;
	inline static std::vector<Car*> vector;
	inline static I::Class* pClass;
};