#pragma once
#include "../main.h"

#include "PickItemDataConfig.h"
#include "SOWeaponControl.h"

struct PickItemNet : II::Object {
	inline static IF::Variable<II::Vector3, PickItemNet> SyncPoint;
	inline static IF::Variable<PickItemDataConfig*, PickItemNet> MyPickItemData;
	inline static IF::Variable<int, PickItemNet> BulletNum;

	static void Init() {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("PickItemNet");
		SyncPoint.Init(pClass->Get<IF>("SyncPoint"));
		MyPickItemData.Init(pClass->Get<IF>("MyPickItemData"));
		BulletNum.Init(pClass->Get<IF>("BulletNum"));

	}
	inline static I::Class* pClass;
};
