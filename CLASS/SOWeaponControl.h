#pragma once
#include "BulletSpeedAndGravity.h"

struct SOWeaponControl {
	inline static IF::Variable<bool, SOWeaponControl> HasAutoFire;
	inline static IF::Variable<II::Array<BulletSpeedAndGravity*>*, SOWeaponControl> bulletSpeedAndGravity;

	static void Init() {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("SOWeaponControl");
		HasAutoFire.Init(pClass->Get<IF>("HasAutoFire"));
		bulletSpeedAndGravity.Init(pClass->Get<IF>("bulletSpeedAndGravity"));

		BulletSpeedAndGravity::Init();
	}
	inline static I::Class* pClass;
};
