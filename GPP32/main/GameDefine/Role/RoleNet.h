#pragma once

#include "../../main.h"
#include "../Network/NetworkBase.h"
#include "Role.h"

struct RoleNet : NetworkBase {

	UnityResolve::UnityType::Transform* myTransform;
	UnityResolve::UnityType::GameObject* myGameObj;
	char loops[0x000004];
	char roleNetClient[0x000004];
	char roleNetServer[0x000004];
	RoleLogic* MyRole;
	bool IsLookRole;
	char IsLookRole_[0x000003];
	RoleLogic* _lookRoleInfo;
	char mirror[0x000004];
	char myStateData[0x000004];

	static void Init() {
		pClass = I::Get("Assembly-CSharp.dll")->Get("RoleNet");
		if (pClass) {
			getSyncHp = pClass->Get<IM>("get_SyncHp")->Cast<float, RoleNet*>();
			setSyncHp = pClass->Get<IM>("set_SyncHp")->Cast<void, RoleNet*, float>();

			// veh::Hook(getSyncHp, GetSyncHp);
			// veh::Hook(setSyncHp, SetSyncHp);
		}
	}

	static float GetSyncHp(RoleNet* _this) {
		std::lock_guard lock(Role::mutex);
		try {
			if (_this->MyRole == Role::localRole->roleLogic) {
				return 100.0f;
			}
		} catch (...) {

		}
		return veh::CallOriginal<float>(getSyncHp, _this);
	}

	static void SetSyncHp(RoleNet* _this, float value) {
		std::lock_guard lock(Role::mutex);
		try {
			if (_this->MyRole == Role::localRole->roleLogic) {
				value = 100.0f;
			}
		}
		catch (...) {

		}
		return veh::CallOriginal<void>(setSyncHp, _this, value);
	}

	inline static I::Class* pClass;
	inline static IM::MethodPointer<float, RoleNet*> getSyncHp;
	inline static IM::MethodPointer<void, RoleNet*, float> setSyncHp;
};