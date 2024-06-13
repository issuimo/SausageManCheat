#pragma once
#include "../main.h"

struct PickItemDataConfig : II::Object {
	inline static IF::Variable<II::String*, PickItemDataConfig> itemName;

	static void Init() {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("PickItemDataConfig");
		itemName.Init(pClass->Get<IF>("itemName"));

	}
	inline static I::Class* pClass;
};
