#pragma once

#include "../main.h"

struct AnimatorControl : II::MonoBehaviour {
	inline static IF::Variable<II::Animator*, AnimatorControl> animator;

	static void Init() {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("AnimatorControl");
		animator.Init(pClass->Get<IF>("animator"));
	}

	inline static I::Class* pClass;
};