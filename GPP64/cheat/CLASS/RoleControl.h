#pragma once
#include "../main.h"

#include "AnimatorControl.h"

struct RoleControl {
	inline static IF::Variable<AnimatorControl*, RoleControl> animatorControl;
	inline static IF::Variable<float, RoleControl> y_camRot;

	static void Init() {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("RoleControl");
		animatorControl.Init(pClass->Get<IF>("animatorControl"));
		y_camRot.Init(pClass->Get<IF>("y_camRot"));
	}

	inline static I::Class* pClass;
};