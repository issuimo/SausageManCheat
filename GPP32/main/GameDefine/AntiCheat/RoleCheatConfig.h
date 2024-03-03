#pragma once
#include "../../main.h"

struct RoleCheatConfig : II::Object {

	char checkRole[0x000004];
	bool isCheckCharacterSize;
	char isCheckCharacterSize_[0x000003];
	int AdsorbCheadCount;
	int lastCheckKillNum;
	char isCloseCheckConfig[0x000004];

	static void AntiStartDetection() {
		pClass = I::Get("Assembly-CSharp.dll")->Get("RoleCheatConfig");
		checkRoleSize = pClass->Get<IM>("CheckRoleSize")->Cast<void, RoleCheatConfig*, float, std::int64_t>();
		veh::Hook(checkRoleSize, CheckRoleSize);
	}

	static void CheckRoleSize(RoleCheatConfig* _this, float diffY, std::int64_t checkPlayerId) {
		LOG_WARNING(std::format("RoleCheatConfig->CheckRoleSize\n"));
		return;
	}

	inline static  IC* pClass;
	inline static IM::MethodPointer<void, RoleCheatConfig*, float, std::int64_t> checkRoleSize;
};