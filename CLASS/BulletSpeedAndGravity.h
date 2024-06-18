#pragma once

class BulletSpeedAndGravity {
public:
	inline static IF::Variable<float, BulletSpeedAndGravity> gravity;
	inline static IF::Variable<float, BulletSpeedAndGravity> bulletSpeed;

	static void Init() {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("BulletSpeedAndGravity");
		gravity.Init(pClass->Get<IF>("gravity"));
		bulletSpeed.Init(pClass->Get<IF>("bulletSpeed"));

	}
	inline static I::Class* pClass;
};
