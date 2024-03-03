#pragma once
#include "../../main.h"
#include "../Role/RoleLogicServer.h"
#include "RoleCheatAction.h"
#include "RoleCheatBullet.h"
#include "RoleCheatBulletFireData.h"
#include "RoleCheatCar.h"
#include "RoleCheatCollider.h"
#include "RoleCheatConfig.h"
#include "RoleCheatHolySword.h"
#include "RoleCheatJump.h"
#include "RoleCheatKill.h"
#include "RoleCheatMeleeAttackData.h"
#include "RoleCheatMove.h"
#include "RoleCheatMTP.h"
#include "RoleCheatPoint.h"
#include "RoleCheatUseItem.h"
#include "RoleCheatWave.h"
#include "RoleCheatWeapon.h"

struct RoleCheat : II::Object {

	RoleLogicServer* CheckRoleLogic;
	RoleCheatMove* MyRoleCheatMove;
	RoleCheatConfig* MyRoleCheatConfig;
	RoleCheatMTP* MyRoleCheatMTP;
	RoleCheatCollider* MyRoleCheatCollider;
	RoleCheatUseItem* MyRoleCheatUseItem;
	RoleCheatBullet* MyRoleCheatBullet;
	RoleCheatAction* MyRoleCheatAction;
	RoleCheatCar* MyRoleCheatCar;
	RoleCheatWeapon* MyRoleCheatWeapon;
	RoleCheatBulletFireData* MyRoleCheatBulletFireData;
	RoleCheatMeleeAttackData* RoleCheatMeleeAttackData;
	RoleCheatHolySword* RoleCheatHolySword;
	RoleCheatKill* RoleCheatKill;
	RoleCheatJump* RoleCheatJump;
	RoleCheatPoint* RoleCheatPoint;
	RoleCheatWave* RoleCheatWave;
	char loops[0x000004];


	static void AntiStartDetection() {
		pClass = I::Get("Assembly-CSharp.dll")->Get("RoleCheat");
		onFixedUpdate = pClass->Get<IM>("OnFixedUpdate")->Cast<void, RoleCheat*, float>();
		checkJumpValue = pClass->Get<IM>("CheckJumpValue")->Cast<void, RoleCheat*, float>();
		setCheatMaxSpeed = pClass->Get<IM>("SetCheatMaxSpeed")->Cast<void, RoleCheat*, float, float>();
		checkClientPos = pClass->Get<IM>("CheckClientPos")->Cast<void, RoleCheat*, II::Vector3, II::Vector3>();
		checkWaveCanDamage = pClass->Get<IM>("CheckWaveCanDamage")->Cast<void, RoleCheat*, II::String*, int, int, int>();
		checkRoleSize = pClass->Get<IM>("CheckRoleSize")->Cast<void, RoleCheat*, float, std::int64_t>();
		veh::Hook(onFixedUpdate, OnFixedUpdate);
		veh::Hook(checkJumpValue, CheckJumpValue);
		veh::Hook(setCheatMaxSpeed, SetCheatMaxSpeed);
		veh::Hook(checkClientPos, CheckClientPos);
		veh::Hook(checkWaveCanDamage, CheckWaveCanDamage);
		veh::Hook(checkRoleSize, CheckRoleSize);
	}

	static void OnFixedUpdate(RoleCheat* _this, float delta) {
		LOG_WARNING("RoleCheat->OnFixedUpdate is call \n");
		return;
	}

	static void SetCheatMaxSpeed(RoleCheat* _this, float maxSpeed, float useTime) {
		LOG_WARNING(std::format("RoleCheat->SetCheatMaxSpeed args->maxSpeed:{} useTime:{}\n", maxSpeed, useTime));
		return;
	}

	static void CheckJumpValue(RoleCheat* _this, float value) {
		LOG_WARNING(std::format("RoleCheat->CheckJumpValue args->value:{}\n", value));
		return;
	}

	static void CheckClientPos(RoleCheat* _this, II::Vector3 roleClientPos, II::Vector3 roleClientRoleNetPos) {
		LOG_WARNING(std::format("RoleCheat->CheckClientPos\n"));
		return;
	}

	static bool CheckWaveCanDamage(RoleCheat* _this, II::String* weaponSign, int waveIndex, int roleIndex, int attackType) {
		LOG_WARNING(std::format("RoleCheat->CheckWaveCanDamage WeaponSign:{}, AttackType:{}\n", weaponSign->ToString(), attackType));
		return true;
	}

	static void CheckRoleSize(RoleCheat* _this, float diffY, std::int64_t checkPlayerId) {
		LOG_WARNING(std::format("RoleCheat->CheckRoleSize\n"));
		return;
	}

	inline static  IC* pClass;
	inline static IM::MethodPointer<void, RoleCheat*, float> onFixedUpdate;
	inline static IM::MethodPointer<void, RoleCheat*, float> checkJumpValue;
	inline static IM::MethodPointer<void, RoleCheat*, float, float> setCheatMaxSpeed;
	inline static IM::MethodPointer<void, RoleCheat*, II::Vector3, II::Vector3> checkClientPos;
	inline static IM::MethodPointer<void, RoleCheat*, II::String*, int, int, int> checkWaveCanDamage;
	inline static IM::MethodPointer<void, RoleCheat*, float, std::int64_t> checkRoleSize;
};