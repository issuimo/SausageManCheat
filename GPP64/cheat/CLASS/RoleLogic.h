#pragma once
#include "../main.h"

#include "PickItemNet.h"

struct RoleLogic : II::MonoBehaviour {
	inline static IF::Variable<II::String*, RoleLogic> NickName;
	inline static IF::Variable<bool, RoleLogic> isLocalRole;
	inline static IF::Variable<float, RoleLogic> hp;
	inline static IF::Variable<int64_t, RoleLogic> TeamNum;
	inline static IF::Variable<float, RoleLogic> WeakValue;
	inline static IF::Variable<II::Vector3, RoleLogic> NowPoint;
	inline static IF::Variable<float, RoleLogic> maxHp;
	inline static IF::Variable<PickItemNet*, RoleLogic> UserWeapon;
	inline static IF::Variable<int, RoleLogic> KillRoleNum;
	inline static IF::Variable<PickItemNet*, RoleLogic> HeadEquipPart;
	inline static IF::Variable<PickItemNet*, RoleLogic> BodyEquipPart;
	inline static IF::Variable<PickItemNet*, RoleLogic> PackEquip;

	static void Init() {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("RoleLogic");
		NickName.Init(pClass->Get<IF>("NickName"));
		isLocalRole.Init(pClass->Get<IF>("isLocalRole"));
		hp.Init(pClass->Get<IF>("hp"));
		TeamNum.Init(pClass->Get<IF>("TeamNum"));
		WeakValue.Init(pClass->Get<IF>("WeakValue"));
		NowPoint.Init(pClass->Get<IF>("NowPoint"));
		maxHp.Init(pClass->Get<IF>("maxHp"));
		UserWeapon.Init(pClass->Get<IF>("UserWeapon"));
		KillRoleNum.Init(pClass->Get<IF>("KillRoleNum"));
		HeadEquipPart.Init(pClass->Get<IF>("HeadEquipPart"));
		BodyEquipPart.Init(pClass->Get<IF>("BodyEquipPart"));
		PackEquip.Init(pClass->Get<IF>("PackEquip"));
	}

	inline static I::Class* pClass;
};